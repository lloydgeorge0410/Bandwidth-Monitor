import winston from "winston";

export const logMessageFormat = (info: winston.Logform.TransformableInfo) => 
  `${info.timestamp} ${info.level}: ${info.message}${info.stack ? " - " + info.stack : ""}`;
