import "reflect-metadata";
import { createDatabase } from "typeorm-extension";

import Logger from "../utils/logger";
import AppDataSource from "./config";

export const initializeDataSource = async () => {
  try {
    await createDatabase({ options: AppDataSource.options, ifNotExist: true });
    await AppDataSource.initialize();
    await AppDataSource.runMigrations();
    Logger.info("Initialized data source successfully.");
  } catch (err) {
    Logger.error("Error during Data Source initialization", err);
  }
};
