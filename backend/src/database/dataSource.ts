import "reflect-metadata";
import { DataSource, DataSourceOptions } from "typeorm";
import { createDatabase } from "typeorm-extension";

import { ENV } from "../utils/constants";
import Logger from "../utils/logger";

const options: DataSourceOptions = {
  type: "postgres",
  host: ENV.DB_HOST,
  port: Number(ENV.DB_PORT),
  username: ENV.DB_USER,
  password: ENV.DB_PASSWORD,
  database: ENV.DB_NAME,
  entities: [ "entities/*.ts" ],
};

const AppDataSource = new DataSource(options);

export const initializeDataSource = async () => {
  try {
    await createDatabase({ options: options, ifNotExist: true });
    await AppDataSource.initialize();
    await AppDataSource.synchronize();
    Logger.info("Initialized data source successfully.");
  } catch (err) {
    Logger.error("Error during Data Source initialization", err);
  }
};

export default AppDataSource;
