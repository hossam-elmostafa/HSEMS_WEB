/**
 * Helper function to update Menu object - apply disabled tags
 * @param {object} menuObj - Menu object
 * @param {string} disabledTag - Tag to disable
 * @returns {boolean} - True if tag was found and disabled
 */
export function applayDisabledTags(menuObj, disabledTag) {
  if (menuObj.children) {
    for (var i = 0; i < menuObj.children.length; i++) {
      if (applayDisabledTags(menuObj.children[i], disabledTag)) return true;
    }
  } else {
    for (var prop in menuObj) {
      if (prop === "TagName") {
        if (menuObj[prop] === disabledTag) {            
            menuObj["disabled"] = true;            
          return true;
        }
      }
    }
  }
  return false;
}





