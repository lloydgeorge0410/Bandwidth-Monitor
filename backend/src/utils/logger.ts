import winston from "winston";
import "winston-daily-rotate-file";
import { logMessageFormat } from "./helper";

const levels = {
  error: 0,
  warn: 1,
  info: 2,
  http: 3,
  debug: 4,
};

const colors = {
  error: "red",
  warn: "yellow",
  info: "green",
  http: "magenta",
  debug: "white",
};

winston.addColors(colors);

const format = winston.format.combine(
  winston.format.timestamp({ format: "YYYY-MM-DD HH:mm:ss:ms" }),
  winston.format.prettyPrint(),
  winston.format.errors({ stack: true }),
);

const consoleFormat = winston.format.combine(
  winston.format.colorize({ all: true }),
  winston.format.timestamp({ format: "YYYY-MM-DD HH:mm:ss:ms" }),
  winston.format.errors({ stack: true }),
  winston.format.printf(logMessageFormat),
);

const transports = [
  new winston.transports.Console({
    format: consoleFormat,
  }),
  new winston.transports.DailyRotateFile({
    filename: "logs/%DATE%.log",
    datePattern: "YYYY-MM-DD",
    maxSize: "20m",
    format: format,
  }),
];

const exceptionHandlers = [
  new winston.transports.DailyRotateFile({
    filename: "logs/exception_%DATE%.log",
    datePattern: "YYYY-MM-DD",
    maxSize: "20m",
    format: format,
  }),
];

const rejectionHandlers = [
  new winston.transports.DailyRotateFile({
    filename: "logs/rejections_%DATE%.log",
    datePattern: "YYYY-MM-DD",
    maxSize: "20m",
    format: format,
  }),
];

const Logger = winston.createLogger({
  level: "debug",
  format: winston.format.prettyPrint(),
  levels,
  transports,
  exceptionHandlers,
  rejectionHandlers,
});

export default Logger;
