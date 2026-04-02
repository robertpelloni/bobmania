# Fix for Chamber.Law.Frontend Build Failure

The build logs indicate missing peer dependencies for `Turbopack` / `Next.js`. The following packages are imported but not resolved:

1.  **@fullcalendar/core**: Required by `@fullcalendar/react`, `@fullcalendar/daygrid`, etc.
2.  **rxjs**: Required by `@apollo/client`.
3.  **y-protocols**: Required by `@tiptap/y-tiptap` and `y-prosemirror`.

## Recommended Fix

Run the following command in the root of the `Chamber.Law.Frontend` repository:

```bash
npm install @fullcalendar/core rxjs y-protocols
```

## Explanation
- **FullCalendar:** The React adapter and plugins expect the core library to be present.
- **Apollo Client:** Recent versions of Apollo Client have `rxjs` as a peer dependency for Observables.
- **TipTap Collaboration:** Uses `y-protocols` for Awareness sync.

## Note on Environment
This file was generated within the **Unified StepMania** workspace. The `Chamber.Law.Frontend` files are not accessible here.
