CREATE TABLE IF NOT EXISTS demo(id int primary key, name text);
INSERT INTO demo VALUES (1,'a') ON CONFLICT DO NOTHING;

DELETE FROM demo;                 -- ❌ Blocked
DELETE FROM demo /* CONFIRM */;   -- ✅ Allowed
UPDATE demo SET name='x';         -- ❌ Blocked
UPDATE demo SET name='z' WHERE id=1; -- ✅ Allowed
