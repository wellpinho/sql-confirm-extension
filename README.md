# SQL Confirm Extension (Experimental) 🛡️

> Experimental safety layer for SQL that blocks DELETE/UPDATE without WHERE unless CONFIRM is explicitly used.

Contains:

- PostgreSQL server extension (C)
- Node.js CLI linter for CI/pre-commit

Motivation:
Prevent accidental mass operations that can cause critical data loss.
