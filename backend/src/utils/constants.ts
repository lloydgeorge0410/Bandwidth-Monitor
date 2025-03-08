import dotenv from "dotenv";

dotenv.config();

export const ENV = {
  HOST: process.env.HOST ?? "127.0.0.1",
  PORT: Number(process.env.PORT ?? "5000"),
};
