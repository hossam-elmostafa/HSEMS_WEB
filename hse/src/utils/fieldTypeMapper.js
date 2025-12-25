/**
 * Field Type Mapper Utility
 * 
 * Maps UNKNOWN field types to appropriate types based on field properties.
 * This utility fixes fields that come from the server with Type: 'UNKNOWN'
 * 
 * Usage: Called from ShowScreen event to fix field types when screen loads
 */

/**
 * Map UNKNOWN field types to appropriate types based on field properties
 * @param {string} strScrTag - Screen tag
 * @param {string} strTabTag - Tab tag (if applicable)
 * @param {Object} devInterface - Object containing devInterface functions (changeFldObj, etc.)
 */
export async function mapUnknownFieldTypes(strScrTag, strTabTag, devInterface) {
  if (!devInterface || !devInterface.changeFldObj) {
    console.warn('[Web_HSE] changeFldObj not available for field type mapping');
    return;
  }

  const scrType = strTabTag && strTabTag !== '' ? 'tab' : 'scr';

  // Field type mappings based on field name patterns and properties
  // Format: { fieldName: { type: 'FieldType', ...otherProps } }
  const fieldTypeMappings = {
    // NRSTMISCENT_SGSTDACTNS - Further Actions Required
    // DataMaxLength: 1024 indicates this should be a MemoText field
    'NRSTMISCENT_SGSTDACTNS': {
      type: 'MemoText',
      scrType: scrType,
    },
  };

  try {
    // Apply field type mappings
    for (const [fieldName, mapping] of Object.entries(fieldTypeMappings)) {
      try {
        const { type, scrType: fieldScrType = scrType, ...otherProps } = mapping;
        
        // Call changeFldObj to update the field type
        devInterface.changeFldObj({
          scrType: fieldScrType,
          fldDBNam: fieldName,
          newFldType: type,
          ...otherProps,
        });

        console.log(`[Web_HSE] Mapped field ${fieldName} from UNKNOWN to ${type}`);
      } catch (error) {
        console.warn(`[Web_HSE] Error mapping field ${fieldName}:`, error);
      }
    }
  } catch (error) {
    console.error('[Web_HSE] Error in mapUnknownFieldTypes:', error);
  }
}

/**
 * Check if a field should be mapped based on its properties
 * This is a helper function for dynamic field type detection
 * @param {Object} fieldObj - Field object from screen structure
 * @returns {string|null} - Suggested field type or null
 */
export function suggestFieldType(fieldObj) {
  if (!fieldObj || fieldObj.Type !== 'UNKNOWN') {
    return null;
  }

  const dataMaxLength = fieldObj.DataMaxLength || 0;
  const fieldName = (fieldObj.DataFieldName || '').toUpperCase();

  // MemoText for long text fields (typically > 255 characters)
  if (dataMaxLength > 255) {
    return 'MemoText';
  }

  // Edit for shorter text fields
  if (dataMaxLength > 0 && dataMaxLength <= 255) {
    return 'edit';
  }

  // Default to edit if no other criteria match
  return 'edit';
}

