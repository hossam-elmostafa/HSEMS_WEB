import { Request, Response } from 'express';
import { appConfig } from '../config/appConfig';

/**
 * Health check endpoint
 * Returns service status
 */
export const getHealth = (req: Request, res: Response) => {
  res.json({ 
    status: 'ok', 
    service: appConfig.serviceName 
  });
};

