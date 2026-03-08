/**
 * Create minimal screen handlers for all screens under remaining HSE menus
 * (Inquiry screens, Logs, Reports, etc.) that don't already have a handler.
 * Skips: File, Setup, Environment, Safety, Performance (already done).
 */
const fs = require('fs');
const path = require('path');

const HSE_JSON_PATH = path.join(__dirname, '../../../Webinfra.Server/AppMenus/HSE.json');
const SCREEN_HANDLERS_ROOT = path.join(__dirname, '../src/screenHandlers');
const INDEX_PATH = path.join(__dirname, '../src/screenHandlers/index.js');

const DONE_MENUS = new Set(['File', 'Setup', 'Environment', 'Safety', 'Performance']);

function captionToFileName(caption) {
  return caption.replace(/\s+/g, '_').replace(/[^\w\-]/g, '') + '.js';
}

function collectScreensWithTag(node, pathSegments, out, isRoot) {
  if (!node) return;
  if (node.TagName && node.English_Caption) {
    out.push({
      tag: node.TagName,
      pathSegments: [...pathSegments],
      caption: node.English_Caption,
    });
  }
  if (Array.isArray(node.children)) {
    const nextSegments = isRoot ? pathSegments : (node.English_Caption ? [...pathSegments, node.English_Caption] : pathSegments);
    for (const child of node.children) {
      collectScreensWithTag(child, nextSegments, out, false);
    }
  }
}

function getExistingTagsFromIndex() {
  const content = fs.readFileSync(INDEX_PATH, 'utf8');
  const match = content.match(/const SCREEN_HANDLERS = \{[\s\S]*?\};/);
  if (!match) return new Set();
  const block = match[0];
  const tags = new Set();
  const re = /\s*'([^']+)':\s*\w+,?/g;
  let m;
  while ((m = re.exec(block)) !== null) {
    tags.add(m[1]);
  }
  return tags;
}

const handlerTemplate = (tag, menuPath, description) => `/**
 * Screen handler: ${description} (${tag})
 * Menu path: ${menuPath} (from HSE.json)
 * Minimal handler: enable toolbar on ShowScreen.
 */

export async function ShowScreen(setScreenDisableBtn, strScrTag, strTabTag, devInterfaceObj) {
  setScreenDisableBtn(false, false, false);
}
`;

function main() {
  const menu = JSON.parse(fs.readFileSync(HSE_JSON_PATH, 'utf8'));
  const hseChildren = menu?.Menu?.children?.[0]?.children;
  if (!Array.isArray(hseChildren)) {
    console.error('HSE menu children not found');
    process.exit(1);
  }

  const existingTags = getExistingTagsFromIndex();
  const allScreens = [];

  for (const menuNode of hseChildren) {
    const menuCaption = menuNode.English_Caption;
    if (!menuCaption || DONE_MENUS.has(menuCaption)) continue;
    const screens = [];
    collectScreensWithTag(menuNode, [], screens, true);
    for (const s of screens) {
      allScreens.push({ ...s, menuCaption });
    }
  }

  const toAdd = allScreens.filter((s) => !existingTags.has(s.tag));
  console.log(`Found ${allScreens.length} screens in remaining menus, ${toAdd.length} need handlers (${allScreens.length - toAdd.length} already registered).`);

  const imports = [];
  const entries = [];

  for (const { tag, pathSegments, caption, menuCaption } of toAdd) {
    const relFolder = pathSegments.length ? menuCaption + '/' + pathSegments.join('/') : menuCaption;
    const fileName = captionToFileName(caption);
    const dirPath = path.join(SCREEN_HANDLERS_ROOT, relFolder);
    const filePath = path.join(dirPath, fileName);
    const menuPath = menuCaption + (pathSegments.length ? ' -> ' + pathSegments.join(' -> ') + ' -> ' : ' -> ') + caption;
    const importVar = tag.replace(/[^a-zA-Z0-9]/g, '_');
    const relImport = './' + relFolder.replace(/\\/g, '/') + '/' + fileName;

    fs.mkdirSync(dirPath, { recursive: true });
    fs.writeFileSync(filePath, handlerTemplate(tag, menuPath, caption), 'utf8');
    console.log('Created', relImport);
    imports.push(`import * as ${importVar} from '${relImport}';`);
    entries.push(`  '${tag}': ${importVar},`);
  }

  console.log('\n--- Add these to screenHandlers/index.js ---\n');
  console.log(imports.join('\n'));
  console.log('\nIn SCREEN_HANDLERS add:\n');
  console.log(entries.join('\n'));
}

main();
