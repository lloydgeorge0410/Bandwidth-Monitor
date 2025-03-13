import { DataSource } from "typeorm";
import { WinstonAdaptor } from "typeorm-logger-adaptor";

import { ENV } from "../utils/constants";
import Logger from "../utils/logger";

export default new DataSource({
  type: "postgres",
  host: ENV.DB_HOST,
  port: Number(ENV.DB_PORT),
  username: ENV.DB_USER,
  password: ENV.DB_PASSWORD,
  database: ENV.DB_NAME,
  entities: ["src/entities/**/*.ts"],
  migrations: ["src/migrations/**/*.ts"],
  logging: true,
  logger: new WinstonAdaptor(Logger, "all"),
});
