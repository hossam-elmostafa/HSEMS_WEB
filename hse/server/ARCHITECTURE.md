# Web_HSE Server Architecture

## Overview

The Web_HSE server has been refactored from a single-file structure into a **modular, maintainable architecture** following Express.js best practices.

## Directory Structure

```
server/src/
├── index.ts                    # Entry point - starts the server
├── app.ts                      # Express app configuration
│
├── config/                     # Configuration
│   ├── appConfig.ts           # App settings (port, env, service name)
│   └── corsOptions.ts         # CORS configuration
│
├── controllers/                # Business logic
│   ├── buttonClickController.ts    # Handles button click logging
│   └── healthController.ts         # Handles health checks
│
├── middleware/                 # Express middleware
│   ├── errorHandler.ts        # Global error handler
│   ├── notFoundHandler.ts     # 404 handler
│   └── validateButtonClick.ts # Request validation
│
├── routes/                     # Route definitions
│   ├── index.ts               # Main router (combines all routes)
│   ├── observationRoutes.ts   # /api/observation/* routes
│   └── healthRoutes.ts        # /health routes
│
└── types/                      # TypeScript types
    └── buttonClick.ts          # Request/response interfaces
```

## Architecture Principles

### 1. Separation of Concerns

- **Routes** (`routes/`) - Define URL endpoints and HTTP methods only
- **Controllers** (`controllers/`) - Handle business logic and request processing
- **Middleware** (`middleware/`) - Handle cross-cutting concerns (validation, errors)
- **Config** (`config/`) - Centralized configuration
- **Types** (`types/`) - TypeScript type definitions for type safety

### 2. Single Responsibility

Each file has a single, well-defined purpose:
- `buttonClickController.ts` - Only handles button click logging
- `validateButtonClick.ts` - Only validates button click requests
- `errorHandler.ts` - Only handles errors

### 3. Modularity

- Easy to add new features (just add new routes/controllers)
- Easy to test (each module can be tested independently)
- Easy to maintain (changes are isolated to specific modules)

## Request Flow

```
Client Request
    ↓
Express App (app.ts)
    ↓
Routes (routes/index.ts)
    ↓
Route Handler (routes/observationRoutes.ts)
    ↓
Middleware (middleware/validateButtonClick.ts) - Validates request
    ↓
Controller (controllers/buttonClickController.ts) - Processes request
    ↓
Response to Client
```

## Adding New Features

### Example: Adding a New Endpoint

**1. Create Controller** (`src/controllers/myFeatureController.ts`):
```typescript
import { Request, Response } from 'express';

export const myFeatureHandler = (req: Request, res: Response) => {
  // Your business logic here
  res.json({ success: true });
};
```

**2. Create Route** (`src/routes/myFeatureRoutes.ts`):
```typescript
import { Router } from 'express';
import { myFeatureHandler } from '../controllers/myFeatureController';

const router = Router();
router.get('/my-endpoint', myFeatureHandler);
export default router;
```

**3. Register Route** (`src/routes/index.ts`):
```typescript
import myFeatureRoutes from './myFeatureRoutes';
router.use('/api/my-feature', myFeatureRoutes);
```

That's it! The new endpoint is now available at `/api/my-feature/my-endpoint`.

## Benefits of This Structure

✅ **Maintainability** - Easy to find and modify code
✅ **Scalability** - Easy to add new features without cluttering
✅ **Testability** - Each module can be tested independently
✅ **Type Safety** - TypeScript types ensure correctness
✅ **Consistency** - Follows Express.js best practices
✅ **Readability** - Clear separation makes code easier to understand

## Comparison: Before vs After

### Before (Single File)
```
src/index.ts (69 lines)
  - CORS config
  - Routes
  - Controllers
  - Middleware
  - Error handling
  - Server startup
```

### After (Modular)
```
src/
  - index.ts (15 lines) - Just starts server
  - app.ts (25 lines) - App configuration
  - config/ - Configuration files
  - controllers/ - Business logic
  - middleware/ - Request processing
  - routes/ - URL definitions
  - types/ - Type definitions
```

**Result**: Much easier to navigate, maintain, and extend!

