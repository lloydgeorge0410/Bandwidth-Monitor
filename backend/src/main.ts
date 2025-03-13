import app from "./app";
import { initializeDataSource } from "./database/dataSource";
import { ENV } from "./utils/constants";
import Logger from "./utils/logger";

const startServer = async () => {
  initializeDataSource().then(() => {
    app.listen(ENV.PORT, ENV.HOST, () => {
      Logger.info(`Server is running at http://${ENV.HOST}:${ENV.PORT}.`);
    });
  });
};

startServer();
