import { NextFunction, Request, Response } from "express";
import errorHandler from "../../src/middlewares/errorHandler"; // Adjust the import path as needed
import Logger from "../../src/utils/logger";

// Mock the Logger
jest.mock("../../src/utils/logger", () => ({
  error: jest.fn(),
}));

describe("Error Handler Middleware", () => {
  let mockRequest: Partial<Request>;
  let mockResponse: Partial<Response>;
  let nextFunction: NextFunction;
  let testError: Error;

  beforeEach(() => {
    // Reset mocks before each test
    mockRequest = {};
    mockResponse = {
      status: jest.fn().mockReturnThis(),
      send: jest.fn(),
      headersSent: false,
    };
    nextFunction = jest.fn();
    testError = new Error("Test error message");

    // Clear all mock calls
    jest.clearAllMocks();
  });

  it("should pass the error to next if headers are already sent", () => {
    // Setup
    mockResponse.headersSent = true;

    // Execute
    errorHandler(testError, mockRequest as Request, mockResponse as Response, nextFunction);

    // Assert
    expect(nextFunction).toHaveBeenCalledWith(testError);
    expect(Logger.error).not.toHaveBeenCalled();
    expect(mockResponse.status).not.toHaveBeenCalled();
    expect(mockResponse.send).not.toHaveBeenCalled();
  });

  it("should log the error and send a 500 response when headers are not sent", () => {
    // Setup
    mockResponse.headersSent = false;

    // Execute
    errorHandler(testError, mockRequest as Request, mockResponse as Response, nextFunction);

    // Assert
    expect(nextFunction).not.toHaveBeenCalled();
    expect(Logger.error).toHaveBeenCalledWith("Server Error:", testError);
    expect(mockResponse.status).toHaveBeenCalledWith(500);
    expect(mockResponse.send).toHaveBeenCalledWith("Test error message");
  });

  it("should handle errors with empty messages", () => {
    // Setup
    mockResponse.headersSent = false;
    const emptyMessageError = new Error("");

    // Execute
    errorHandler(emptyMessageError, mockRequest as Request, mockResponse as Response, nextFunction);

    // Assert
    expect(mockResponse.status).toHaveBeenCalledWith(500);
    expect(mockResponse.send).toHaveBeenCalledWith("");
  });

  it("should handle non-Error objects", () => {
    // Setup
    mockResponse.headersSent = false;
    const nonErrorObject = { message: "Not a standard error" } as Error;

    // Execute
    errorHandler(nonErrorObject, mockRequest as Request, mockResponse as Response, nextFunction);

    // Assert
    expect(Logger.error).toHaveBeenCalledWith("Server Error:", nonErrorObject);
    expect(mockResponse.status).toHaveBeenCalledWith(500);
    expect(mockResponse.send).toHaveBeenCalledWith(nonErrorObject.message);
  });
});
