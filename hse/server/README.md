# Web_HSE Server

Express + TypeScript backend for HSE button click logging with modular architecture.

## Project Structure

```
server/
├── src/
│   ├── app.ts                    # Express app configuration
│   ├── index.ts                  # Application entry point
│   │
│   ├── config/                   # Configuration files
│   │   ├── appConfig.ts         # Application configuration (port, env, etc.)
│   │   └── corsOptions.ts       # CORS configuration
│   │
│   ├── controllers/              # Business logic / request handlers
│   │   ├── buttonClickController.ts  # Button click logging logic
│   │   └── healthController.ts        # Health check logic
│   │
│   ├── middleware/               # Express middleware
│   │   ├── errorHandler.ts      # Global error handler
│   │   ├── notFoundHandler.ts   # 404 handler
│   │   └── validateButtonClick.ts    # Request validation
│   │
│   ├── routes/                   # Route definitions
│   │   ├── index.ts              # Main router (combines all routes)
│   │   ├── observationRoutes.ts  # Observation-related routes
│   │   └── healthRoutes.ts       # Health check routes
│   │
│   └── types/                    # TypeScript type definitions
│       └── buttonClick.ts        # Button click request/response types
│
├── dist/                         # Compiled JavaScript (after build)
├── package.json
├── tsconfig.json
└── .env                          # Environment variables (create this)
```

## Architecture

The application follows a **modular MVC-like structure**:

- **Routes** (`src/routes/`) - Define URL endpoints and HTTP methods
- **Controllers** (`src/controllers/`) - Handle business logic and request processing
- **Middleware** (`src/middleware/`) - Request validation, error handling, etc.
- **Config** (`src/config/`) - Configuration files (CORS, app settings)
- **Types** (`src/types/`) - TypeScript interfaces and types

## Setup

```powershell
npm install
```

## Development

```powershell
npm run dev
```

Starts the server with hot reload on `http://localhost:7070`

## Production Build

```powershell
npm run build
npm start
```

## Environment Variables

Create a `.env` file (optional):

```env
PORT=7070
NODE_ENV=development
```

Default port is 7070 if not specified.

## API Endpoints

### POST /api/observation/button-click

Logs button clicks from the HSE client.

**Request:**
```json
{
  "buttonName": "NRSTMISCENT_CNCL"
}
```

**Response:**
- `204 No Content` on success
- `400 Bad Request` if buttonName is invalid

**Console Output:**
```
ButtonClicked: "NRSTMISCENT_CNCL"
```

### GET /health

Health check endpoint.

**Response:**
```json
{
  "status": "ok",
  "service": "Web_HSE Server"
}
```

## Adding New Features

### Adding a New Route

1. Create route file in `src/routes/`:
```typescript
// src/routes/myFeatureRoutes.ts
import { Router } from 'express';
import { myFeatureController } from '../controllers/myFeatureController';

const router = Router();
router.get('/my-endpoint', myFeatureController);
export default router;
```

2. Register in `src/routes/index.ts`:
```typescript
import myFeatureRoutes from './myFeatureRoutes';
router.use('/api/my-feature', myFeatureRoutes);
```

### Adding a New Controller

Create controller in `src/controllers/`:
```typescript
// src/controllers/myFeatureController.ts
import { Request, Response } from 'express';

export const myFeatureController = (req: Request, res: Response) => {
  // Your logic here
  res.json({ success: true });
};
```

### Adding Middleware

Create middleware in `src/middleware/`:
```typescript
// src/middleware/myMiddleware.ts
import { Request, Response, NextFunction } from 'express';

export const myMiddleware = (req: Request, res: Response, next: NextFunction) => {
  // Your middleware logic
  next();
};
```

## CORS Configuration

The server allows requests from:
- `http://localhost:3000`
- `http://localhost:3001`
- `http://127.0.0.1:3000`
- `http://127.0.0.1:3001`

To add more origins, edit `src/config/corsOptions.ts`:

```typescript
export const corsOptions: CorsOptions = {
  origin: [
    'http://localhost:3000',
    'http://your-custom-origin:port'
  ],
  // ...
};
```

## Troubleshooting

### TypeScript errors in IDE

Run `npm install` to install type definitions.

### Port already in use

Change the port in `.env`:
```env
PORT=8080
```

Or set it when running:
```powershell
$env:PORT=8080; npm run dev
```

### CORS errors

Check that your React app origin is in the `corsOptions.origin` array in `src/config/corsOptions.ts`.

## Code Style

- Use TypeScript for type safety
- Follow Express.js best practices
- Keep controllers focused on single responsibilities
- Use middleware for cross-cutting concerns (validation, logging, etc.)
- Export types/interfaces from `src/types/` for reusability
