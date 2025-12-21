import { Request, Response, NextFunction } from 'express';
import { ButtonClickRequest } from '../types/buttonClick';

export const validateButtonClick = (
  req: Request,
  res: Response,
  next: NextFunction
) => {
  const { buttonName } = req.body as ButtonClickRequest;

  // Validate input
  if (!buttonName || typeof buttonName !== 'string') {
    return res.status(400).json({ 
      error: 'buttonName is required and must be a string' 
    });
  }

  // Validate max length
  if (buttonName.length > 200) {
    return res.status(400).json({ 
      error: 'buttonName exceeds maximum length (200 characters)' 
    });
  }

  next();
};

