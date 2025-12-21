import { Router } from 'express';
import { logButtonClick } from '../controllers/buttonClickController';
import { validateButtonClick } from '../middleware/validateButtonClick';

const router = Router();

/**
 * POST /api/observation/button-click
 * Logs button clicks from HSE application
 */
router.post('/button-click', validateButtonClick, logButtonClick);

export default router;

