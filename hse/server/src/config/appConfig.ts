import dotenv from 'dotenv';

dotenv.config();

export const appConfig = {
  port: parseInt(process.env.PORT || '7070', 10),
  nodeEnv: process.env.NODE_ENV || 'development',
  serviceName: 'Web_HSE Server',
};

