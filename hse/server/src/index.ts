import { createApp } from './app';
import { appConfig } from './config/appConfig';

/**
 * Application entry point
 */
const startServer = () => {
  const app = createApp();

  app.listen(appConfig.port, () => {
    console.log(`🚀 ${appConfig.serviceName} running on http://localhost:${appConfig.port}`);
    console.log(`📝 Button click logging endpoint: POST http://localhost:${appConfig.port}/api/observation/button-click`);
    console.log(`🏥 Health check endpoint: GET http://localhost:${appConfig.port}/health`);
  });
};

startServer();
