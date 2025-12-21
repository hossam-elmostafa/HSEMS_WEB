# Web_HSE - HSE Button Click Logging

This project implements button click logging for the HSE (Health, Safety, Environment) application without modifying the existing `WebInfra.Client-master` or `Webinfra.Server-master` codebases.

## Architecture

The solution consists of two parts, both located in the `hse` folder:

1. **HSE Client Package** (`Web_HSE/hse/`) - A local npm package that replaces the existing `hse` module via `npm link`
2. **Express Backend** (`Web_HSE/hse/server/`) - A TypeScript Express server that logs button clicks

```
WebInfra.Client-master (React)
    ↓ (imports useHSE from 'hse')
Web_HSE/hse/ (linked package)
    ↓ (POST /api/observation/button-click)
Web_HSE/hse/server/ (Express backend)
    ↓ (console.log)
ButtonClicked: "<button name>"
```

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

## How It Works

### Client Side (`Web_HSE/hse/hse.js`)

The `ButtonClicked` function intercepts button clicks:

```javascript
function ButtonClicked(eventObj) {
  let { Button_Name, strScrTag, ... } = eventObj;
  Button_Name = Button_Name.toString().toUpperCase();
  
  // Send to backend (fire-and-forget)
  sendButtonClickToBackend(Button_Name);
}
```

The `sendButtonClickToBackend` function makes a POST request:

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

### Backend Side (`Web_HSE/hse/server/src/index.ts`)

The Express server receives the request and logs it:

```typescript
app.post('/api/observation/button-click', (req, res) => {
  const { buttonName } = req.body;
  
  // Validate
  if (!buttonName || typeof buttonName !== 'string') {
    return res.status(400).json({ error: 'buttonName is required' });
  }
  
  // Log exactly as specified
  console.log(`ButtonClicked: "${buttonName}"`);
  
  // Return 204 No Content
  res.status(204).send();
});
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

"# HSEMS_WEB" 
