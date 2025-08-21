# SQL Confirm Extension (Experimental) 🛡️

## Experimental safety layer for SQL that blocks DELETE/UPDATE without WHERE unless CONFIRM is explicitly used.

### This repository contains two components:

1. PostgreSQL server extension (C)

- Hooks into the parser to block risky queries.

2. Node.js CLI linter (TypeScript)

- Portable alternative for CI/pre-commit, blocking unsafe SQL.

### Motivation

Accidental mass **_DELETE_** or **_UPDATE_** operations can cause critical data loss.
This project explores an experimental mechanism to require an explicit confirmation **_token CONFIRM_**.

### PostgreSQL Extension (Experimental)

### Requirements

- PostgreSQL headers & development tools (e.g., postgresql-server-dev-16)

- `make`, `gcc`

### Build & Install

```bash
cd postgres-extension
make
sudo make install
```

### Enable the extension in postgresql.conf:

```conf
shared_preload_libraries = 'sql_confirm'
```

Restart PostgreSQL.

### Test in psql

```SQL
CREATE TABLE demo(id int primary key, name text);
INSERT INTO demo VALUES (1,'a'),(2,'b');

-- ❌ Blocked: no WHERE and no CONFIRM
DELETE FROM demo;

-- ✅ Allowed: explicit confirmation
DELETE FROM demo /* CONFIRM */;

-- ✅ Allowed: with WHERE clause
UPDATE demo SET name='x' WHERE id=1;

-- ❌ Blocked: no WHERE and no CONFIRM
UPDATE demo SET name='y';

-- ✅ Allowed: explicit confirmation
UPDATE demo SET name='z' /* CONFIRM */;
```

⚠️ Experimental. Use only in development or testing environments.

### Node.js CLI Linter

Portable linter to detect unsafe SQL queries in CI pipelines or pre-commit hooks.

### Install & Run

```bash
cd node-cli
npm install
npm run build
npm run demo
```

### Usage Example

```ts
import { validateSQL } from "./src/index";

validateSQL("DELETE FROM users"); // ❌ Throws error
validateSQL("DELETE FROM users /* CONFIRM */"); // ✅ Allowed
```

### Contribution

1. Fork the repository

2. Make your changes

3. Submit a Pull Request

Contributions, suggestions, and improvements are welcome.
You can also open issues or start discussions on GitHub.

### Roadmap

- [] Improve PostgreSQL extension to recognize CONFIRM as a proper keyword (not only a comment)

- [] Optional GUC configuration to enable/disable per database

- [] Tests & CI for Node.js CLI

- [] Community feedback and RFC discussion with PostgreSQL developers

### Author

Wellington Pinho
[LinkedIn](linkedin.com/in/wellpinho/)
