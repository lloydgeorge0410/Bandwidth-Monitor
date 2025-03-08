import bodyParser from "body-parser";
import cors from "cors";
import express, { Express } from "express";

import errorHandler from "./middlewares/errorHandler";
import httpLogger from "./middlewares/httpLogger";
import router from "./routes";

const app: Express = express();

app.use(cors());
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: true }));
app.use(httpLogger);
app.use("/", router);
app.use(errorHandler);

export default app;
