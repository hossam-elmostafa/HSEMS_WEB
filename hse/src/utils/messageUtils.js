import { messages } from '../config/constants';

/**
 * Get message by key and replace parameters in the message
 * @param {Function} getCurrentLang - Function to get current language
 * @param {string} key - The key to look up the message
 * @param {any[]} [paramArr=[]] - The array of parameters to replace in the message
 * @returns {string} - The message with parameters replaced
 */
export function getMessage(getCurrentLang, key, paramArr = []) {
  let language = getCurrentLang();
  let message = messages[key]?.[language] || key; // Fallback to key if not found
  paramArr.forEach((param, index) => {
    message = message.replace(`[param${index + 1}]`, param);
  });
  return message;
}





