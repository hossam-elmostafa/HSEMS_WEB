import { Router } from 'express';
import observationRoutes from './observationRoutes';
import healthRoutes from './healthRoutes';

const router = Router();

// Health check route
router.use('/health', healthRoutes);

// Observation routes
router.use('/api/observation', observationRoutes);

export default router;

