# Web_HSE - HSE Button Click Logging

This project implements button click logging for the HSE (Health, Safety, Environment) application without modifying the existing `WebInfra.Client-master` or `Webinfra.Server-master` codebases.

## Table of Contents

- [Application Structure](#application-structure)
- [Architecture Overview](#architecture-overview)
- [Component Details](#component-details)
- [Setup Instructions](#setup-instructions)
- [Running the Application](#running-the-application)
- [Testing](#testing)
- [Key Files](#key-files)
- [How It Works](#how-it-works)
- [Unlinking (Cleanup)](#unlinking-cleanup)
- [Troubleshooting](#troubleshooting)
- [Current Scope](#current-scope)
- [Future Enhancements](#future-enhancements)
- [Quick Reference](#quick-reference)

## Application Structure

```
Web_HSE/
├── README.md                          # This file - main project documentation
│
└── hse/                               # HSE Client Package (npm linkable)
    ├── hse.js                         # Main entry point - exports useHSE hook
    ├── package.json                   # Client package configuration
    │
    ├── src/                           # Client source code
    │   ├── config/
    │   │   └── constants.js           # Configuration constants
    │   │
    │   ├── events/                    # Event handlers
    │   │   ├── buttonEvents.js        # Button click handlers (ButtonClicked, etc.)
    │   │   ├── browseEvents.js        # Browse/table event handlers
    │   │   ├── menuEvents.js          # Menu and app lifecycle events
    │   │   └── screenEvents.js        # Screen/form event handlers
    │   │
    │   ├── services/                  # Business logic services
    │   │   └── ObservationService.js  # Observation-related service logic
    │   │
    │   └── utils/                     # Utility functions
    │       ├── menuUtils.js           # Menu-related utilities
    │       ├── messageUtils.js        # Message/notification utilities
    │       └── policyUtils.js         # Policy/authorization utilities
    │
    └── server/                        # Express Backend Server
        ├── README.md                  # Server-specific documentation
        ├── ARCHITECTURE.md            # Server architecture details
        ├── package.json               # Server dependencies
        ├── tsconfig.json              # TypeScript configuration
        │
        ├── src/                       # TypeScript source code
        │   ├── index.ts               # Application entry point
        │   ├── app.ts                 # Express app configuration
        │   │
        │   ├── config/                # Configuration files
        │   │   ├── appConfig.ts       # App settings (port, env, etc.)
        │   │   └── corsOptions.ts     # CORS configuration
        │   │
        │   ├── controllers/           # Request handlers / business logic
        │   │   ├── buttonClickController.ts  # Button click logging logic
        │   │   └── healthController.ts       # Health check endpoint
        │   │
        │   ├── middleware/            # Express middleware
        │   │   ├── errorHandler.ts    # Global error handler
        │   │   ├── notFoundHandler.ts # 404 handler
        │   │   └── validateButtonClick.ts    # Request validation
        │   │
        │   ├── routes/                # Route definitions
        │   │   ├── index.ts          # Main router (combines all routes)
        │   │   ├── observationRoutes.ts  # Observation API routes
        │   │   └── healthRoutes.ts   # Health check routes
        │   │
        │   └── types/                 # TypeScript type definitions
        │       └── buttonClick.ts    # Button click request/response types
        │
        └── dist/                      # Compiled JavaScript (generated)
            └── [mirrors src/ structure]
```

## Architecture Overview

The solution consists of two main components:

1. **HSE Client Package** (`hse/`) - A local npm package that replaces the existing `hse` module via `npm link`
   - Main entry: `hse.js` exports the `useHSE` hook
   - Event-driven architecture with separate handlers for buttons, menus, screens, and browse events
   - Modular structure with services and utilities

2. **Express Backend** (`hse/server/`) - A TypeScript Express server that logs button clicks
   - MVC-like architecture with routes, controllers, and middleware
   - TypeScript for type safety
   - Modular configuration and error handling

### Data Flow

```
WebInfra.Client-master (React)
    ↓ (imports useHSE from 'hse')
Web_HSE/hse/ (linked package)
    ├── src/events/buttonEvents.js
    │   └── ButtonClicked() → sends POST request
    ↓ (POST /api/observation/button-click)
Web_HSE/hse/server/ (Express backend)
    ├── routes/observationRoutes.ts
    ├── middleware/validateButtonClick.ts
    ├── controllers/buttonClickController.ts
    ↓ (console.log)
ButtonClicked: "<button name>"
```

## Component Details

### HSE Client Package (`hse/`)

The client package is a drop-in replacement for the original `hse` npm package. It provides:

- **Main Entry** (`hse.js`): Exports the `useHSE` hook that integrates with the React application
- **Event Handlers** (`src/events/`):
  - `buttonEvents.js`: Handles button clicks and sends them to the backend
  - `browseEvents.js`: Manages browse/table interactions
  - `menuEvents.js`: Handles menu rendering and clicks
  - `screenEvents.js`: Manages screen/form lifecycle events
- **Services** (`src/services/`): Business logic for observations and other features
- **Utilities** (`src/utils/`): Helper functions for messages, policies, and menus

### Express Backend (`hse/server/`)

A TypeScript Express server with a clean, modular architecture:

- **Entry Point** (`src/index.ts`): Starts the server and loads configuration
- **App Configuration** (`src/app.ts`): Sets up Express middleware and routes
- **Routes** (`src/routes/`): Define API endpoints
- **Controllers** (`src/controllers/`): Handle business logic for each endpoint
- **Middleware** (`src/middleware/`): Request validation, error handling, 404 handling
- **Config** (`src/config/`): Application and CORS configuration
- **Types** (`src/types/`): TypeScript interfaces for type safety

For detailed server documentation, see [`hse/server/README.md`](hse/server/README.md).

## Setup Instructions

### 1. Install Backend Dependencies

```powershell
cd Web_HSE/hse/server
npm install
```

### 2. Link the HSE Client Package

In PowerShell, run:

```powershell
# Step 1: Create a global link for the hse package
cd Web_HSE/hse
npm link

# Step 2: Link the package in the client
cd ../../WebInfra.Client-master
npm link hse
```

**What this does:**
- Creates a symlink from `WebInfra.Client-master/node_modules/hse` to `Web_HSE/hse`
- When the React app imports `hse`, it will use your local version instead of the packaged `.tgz` file
- **No code changes** are made to `WebInfra.Client-master`

### 3. Configure the Client (Optional)

If you want to change the backend URL, create a `.env` file in `WebInfra.Client-master/`:

```env
REACT_APP_WEB_HSE_API=http://localhost:7070
```

Default is `http://localhost:7070` if not specified.

## Running the Application

### Terminal 1: Start the Web_HSE Backend

```powershell
cd Web_HSE/hse/server
npm run dev
```

You should see:
```
🚀 Web_HSE Server running on http://localhost:7070
📝 Button click logging endpoint: POST http://localhost:7070/api/observation/button-click
```

### Terminal 2: Start the Main Backend (Webinfra.Server-master)

```powershell
cd Webinfra.Server-master
npm start
```

### Terminal 3: Start the React Client

```powershell
cd WebInfra.Client-master
npm start
```

## Testing

1. Open the React app in your browser (usually `http://localhost:3000`)
2. Log in and navigate to the HSE application (App Code: 66)
3. Click any custom action button in any HSE screen
4. Check the **Web_HSE backend terminal** - you should see:
   ```
   ButtonClicked: "BUTTON_NAME"
   ```

## Key Files

### Client Package

| File | Purpose |
|------|---------|
| `hse/hse.js` | Main entry point, exports `useHSE` hook |
| `hse/src/events/buttonEvents.js` | Button click event handlers, sends clicks to backend |
| `hse/src/events/menuEvents.js` | Menu rendering and interaction handlers |
| `hse/src/events/screenEvents.js` | Screen/form lifecycle event handlers |
| `hse/src/events/browseEvents.js` | Browse/table event handlers |
| `hse/src/services/ObservationService.js` | Observation business logic |
| `hse/src/utils/messageUtils.js` | Message/notification utilities |
| `hse/src/utils/policyUtils.js` | Policy/authorization utilities |

### Backend Server

| File | Purpose |
|------|---------|
| `hse/server/src/index.ts` | Server entry point, starts Express app |
| `hse/server/src/app.ts` | Express app configuration, middleware setup |
| `hse/server/src/routes/observationRoutes.ts` | Observation API route definitions |
| `hse/server/src/controllers/buttonClickController.ts` | Button click logging logic |
| `hse/server/src/middleware/validateButtonClick.ts` | Request validation middleware |
| `hse/server/src/config/corsOptions.ts` | CORS configuration |
| `hse/server/src/config/appConfig.ts` | Application configuration (port, env) |

## How It Works

### Client Side Flow

1. **Button Click** (`hse/src/events/buttonEvents.js`):
   ```javascript
   function ButtonClicked(eventObj) {
     let { Button_Name, strScrTag, ... } = eventObj;
     Button_Name = Button_Name.toString().toUpperCase();
     
     // Send to backend (fire-and-forget)
     sendButtonClickToBackend(Button_Name);
   }
   ```

2. **HTTP Request** (`hse/src/events/buttonEvents.js`):
   ```javascript
   function sendButtonClickToBackend(buttonName) {
     const url = `${WEB_HSE_API_URL}/api/observation/button-click`;
     
     fetch(url, {
       method: 'POST',
       headers: { 'Content-Type': 'application/json' },
       body: JSON.stringify({ buttonName }),
     })
     .catch((error) => {
       console.warn('[Web_HSE] Failed to log button click:', error.message);
     });
   }
   ```

### Backend Side Flow

1. **Route** (`hse/server/src/routes/observationRoutes.ts`):
   - Defines `POST /api/observation/button-click` endpoint

2. **Validation** (`hse/server/src/middleware/validateButtonClick.ts`):
   - Validates request body structure

3. **Controller** (`hse/server/src/controllers/buttonClickController.ts`):
   ```typescript
   export const logButtonClick = (req: Request, res: Response) => {
     const { buttonName } = req.body;
     
     // Log exactly as specified
     console.log(`ButtonClicked: "${buttonName}"`);
     
     // Return 204 No Content
     res.status(204).send();
   };
   ```

## Unlinking (Cleanup)

When you're done testing and want to restore the original `hse` package:

```powershell
# In WebInfra.Client-master
cd WebInfra.Client-master
npm unlink hse

# Reinstall the original package
npm install
```

This will restore the `hse` package from `modules/hse-1.0.1.tgz`.

## Troubleshooting

### Button clicks not being logged

1. **Check the backend is running**: Visit `http://localhost:7070/health` - you should see `{"status":"ok"}`
2. **Check the link is active**: In `WebInfra.Client-master/node_modules/`, verify that `hse` is a symlink (shows as a shortcut in Windows Explorer)
3. **Check browser console**: Look for `[Web_HSE] Failed to log button click` warnings
4. **Check CORS**: Make sure the React app origin is in the CORS whitelist (`Web_HSE/hse/server/src/index.ts`)

### npm link not working

- Run PowerShell as Administrator
- Delete `node_modules/hse` in the client and try linking again
- Restart your IDE/editor after linking

### TypeScript errors in the backend

```powershell
cd Web_HSE/hse/server
npm install
npm run build
```

## Current Scope

This implementation logs **all custom button clicks** in the HSE application. The logged format is:

```
ButtonClicked: "<BUTTON_ID>"
```

Where `<BUTTON_ID>` is the uppercase button identifier (e.g., `SUBMIT_OBSERVATION`, `APPROVE`, etc.).

## Future Enhancements

- Filter logging to specific Observation screens only (using `eventObj.strScrTag`)
- Add database persistence for button clicks
- Add authentication/authorization
- Implement full Observation business logic
- Add request/response logging middleware

## Quick Reference

### Project Structure Summary

- **Client Package**: `hse/` - React hook package (npm linkable)
- **Backend Server**: `hse/server/` - Express + TypeScript API server
- **Main Entry**: `hse/hse.js` - Exports `useHSE` hook
- **Server Entry**: `hse/server/src/index.ts` - Starts Express server

### Common Commands

```powershell
# Link the client package
cd Web_HSE/hse && npm link
cd ../../WebInfra.Client-master && npm link hse

# Start backend server
cd Web_HSE/hse/server && npm run dev

# Build backend for production
cd Web_HSE/hse/server && npm run build && npm start
```

### API Endpoints

- `POST /api/observation/button-click` - Log button clicks
- `GET /health` - Health check endpoint 
