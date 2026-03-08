/**
 * One-off script: create minimal screen handlers for all Safety menu screens
 * that don't already have a handler. Reads Webinfra.Server/AppMenus/HSE.json,
 * finds Safety subtree, writes handler files and prints index.js additions.
 * Skips Observation Entry, Review, Approval, Reward (already implemented).
 */
const fs = require('fs');
const path = require('path');

const HSE_JSON_PATH = path.join(__dirname, '../../../Webinfra.Server/AppMenus/HSE.json');
const SCREEN_HANDLERS_ROOT = path.join(__dirname, '../src/screenHandlers');
const EXISTING_TAGS = new Set([
  'HSE_TgNrstMiscEnt',
  'HSE_TgNrstMisccnfrmtn',
  'HSE_TgNrstMiscflwup',
  'HSE_TgNrMisRward',
]);

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

function getSafetyNode(menu) {
  const hse = menu?.children?.[0]?.children;
  if (!Array.isArray(hse)) return null;
  return hse.find((n) => n.English_Caption === 'Safety') || null;
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
  const safetyNode = getSafetyNode(menu.Menu);
  if (!safetyNode) {
    console.error('Safety node not found in HSE.json');
    process.exit(1);
  }
  const screens = [];
  collectScreensWithTag(safetyNode, [], screens, true);

  const toAdd = screens.filter((s) => !EXISTING_TAGS.has(s.tag));
  console.log(`Found ${screens.length} Safety screens, ${toAdd.length} need handlers (${screens.length - toAdd.length} already implemented).`);

  const imports = [];
  const entries = [];

  for (const { tag, pathSegments, caption } of toAdd) {
    const relFolder = pathSegments.length ? 'Safety/' + pathSegments.join('/') : 'Safety';
    const fileName = captionToFileName(caption);
    const dirPath = path.join(SCREEN_HANDLERS_ROOT, relFolder);
    const filePath = path.join(dirPath, fileName);
    const menuPath = 'Safety -> ' + (pathSegments.length ? pathSegments.join(' -> ') + ' -> ' : '') + caption;
    const importVar = tag.replace(/[^a-zA-Z0-9]/g, '_');
    const relImport = './' + relFolder.replace(/\\/g, '/') + '/' + fileName;

    fs.mkdirSync(dirPath, { recursive: true });
    fs.writeFileSync(
      filePath,
      handlerTemplate(tag, menuPath, caption),
      'utf8'
    );
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
