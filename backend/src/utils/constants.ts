import dotenv from "dotenv";

dotenv.config();

export const ENV = {
  HOST: process.env.HOST ?? "127.0.0.1",
  PORT: Number(process.env.PORT ?? "5000"),

  DB_HOST: process.env.DB_HOST,
  DB_PORT: process.env.DB_PORT,
  DB_USER: process.env.DB_USER,
  DB_PASSWORD: process.env.DB_PASSWORD,
  DB_NAME: process.env.DB_NAME,

  JWT_SECRET: process.env.JWT_SECRET ?? "",
  JWT_REFRESH_SECRET: process.env.JWT_REFRESH_SECRET ?? "",
};
