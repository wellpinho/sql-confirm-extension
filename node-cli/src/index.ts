function validateSQL(query: string): void {
  const normalized = query.trim().toUpperCase();
  const isDelete = normalized.startsWith("DELETE FROM");
  const isUpdate = normalized.startsWith("UPDATE ");
  const hasWhere = normalized.includes(" WHERE ");
  const hasConfirm = normalized.includes("CONFIRM");

  if ((isDelete || isUpdate) && !hasWhere && !hasConfirm) {
    throw new Error(
      "Operation blocked: DELETE/UPDATE without WHERE. Add CONFIRM to proceed."
    );
  }
}

// Demo runner
const samples = [
  "DELETE FROM users",
  "DELETE FROM users /* CONFIRM */",
  "UPDATE users SET active=false",
  "UPDATE users SET active=false WHERE id=1",
];

for (const q of samples) {
  try {
    validateSQL(q);
    console.log("✅ Allowed:", q);
  } catch (e) {
    console.error("❌ Error:", (e as Error).message, "=>", q);
  }
}
