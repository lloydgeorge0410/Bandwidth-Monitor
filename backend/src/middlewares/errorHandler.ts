import { NextFunction, Request, Response } from "express";

import Logger from "../utils/logger";

const errorHandler = (error: Error, _req: Request, res: Response, next: NextFunction) => {
  if (res.headersSent) return next(error);
  Logger.error("Server Error:", error);
  res.status(500).send(error.message);
};

export default errorHandler;
