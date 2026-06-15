/**
 * RQ_HSE_11_4_26_15_44
 * Escape a value for embedding in a single-quoted T-SQL string literal (legacy EXECUTE / dynamic SQL parity; reduces injection drift).
 * @param {unknown} value
 * @returns {string}
 */
export function escapeSqlStringLiteral(value) {
  if (value == null) return '';
  return String(value).replace(/'/g, "''");
}
