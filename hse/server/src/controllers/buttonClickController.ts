import { Request, Response } from 'express';
import { ButtonClickRequest } from '../types/buttonClick';

/**
 * Handle button click logging
 * Logs the button name to console as specified: ButtonClicked: "<buttonName>"
 */
export const logButtonClick = (req: Request, res: Response) => {
  const { buttonName } = req.body as ButtonClickRequest;

  // Log the button click exactly as specified
  console.log(`ButtonClicked: "${buttonName}"`);

  // Return 204 No Content
  res.status(204).send();
};

