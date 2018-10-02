# calculator-core

This is the Core Business module of this project.

## Public Headers

Public headers are those which:

- Compose your library API
- Should be exported alongside with your libraries
- Should be included for the users of your library

Put them inside `calculator-core/public/`.

## Private Headers

Private headers are those which:

- Are implementation specific
- Doesn't compose your library API
- Shouldn't be exported alongside with your libraries
- Shouldn't be used by the users of your library

Put them inside `calculator-core/src/`.
