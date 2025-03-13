import winston from "winston";
import { logMessageFormat } from "../../src/utils/helper"; // Adjust path as needed

describe("logMessageFormat", () => {
  it("should format a basic log message correctly", () => {
    // Arrange
    const info: winston.Logform.TransformableInfo = {
      timestamp: "2023-01-01 12:00:00",
      level: "info",
      message: "This is an info message",
    };

    // Act
    const result = logMessageFormat(info);

    // Assert
    expect(result).toBe("2023-01-01 12:00:00 info: This is an info message");
  });

  it("should include stack trace when present", () => {
    // Arrange
    const info: winston.Logform.TransformableInfo = {
      timestamp: "2023-01-01 12:00:00",
      level: "error",
      message: "Error occurred",
      stack: "Error: at line 42\n    at function (file.ts:42:10)",
    };

    // Act
    const result = logMessageFormat(info);

    // Assert
    expect(result).toBe(
      "2023-01-01 12:00:00 error: Error occurred - Error: at line 42\n    at function (file.ts:42:10)",
    );
  });

  it("should format warning level messages correctly", () => {
    // Arrange
    const info: winston.Logform.TransformableInfo = {
      timestamp: "2023-01-01 12:00:00",
      level: "warn",
      message: "This is a warning",
    };

    // Act
    const result = logMessageFormat(info);

    // Assert
    expect(result).toBe("2023-01-01 12:00:00 warn: This is a warning");
  });

  it("should format debug level messages correctly", () => {
    // Arrange
    const info: winston.Logform.TransformableInfo = {
      timestamp: "2023-01-01 12:00:00",
      level: "debug",
      message: "Debug information",
    };

    // Act
    const result = logMessageFormat(info);

    // Assert
    expect(result).toBe("2023-01-01 12:00:00 debug: Debug information");
  });

  it("should handle empty message", () => {
    // Arrange
    const info: winston.Logform.TransformableInfo = {
      timestamp: "2023-01-01 12:00:00",
      level: "info",
      message: "",
    };

    // Act
    const result = logMessageFormat(info);

    // Assert
    expect(result).toBe("2023-01-01 12:00:00 info: ");
  });

  it("should handle undefined message", () => {
    // Arrange
    const info: winston.Logform.TransformableInfo = {
      timestamp: "2023-01-01 12:00:00",
      level: "info",
      message: undefined,
    };

    // Act
    const result = logMessageFormat(info);

    // Assert
    expect(result).toBe("2023-01-01 12:00:00 info: undefined");
  });

  it("should handle empty stack trace", () => {
    // Arrange
    const info: winston.Logform.TransformableInfo = {
      timestamp: "2023-01-01 12:00:00",
      level: "error",
      message: "Error with empty stack",
      stack: "",
    };

    // Act
    const result = logMessageFormat(info);

    // Assert
    expect(result).toBe("2023-01-01 12:00:00 error: Error with empty stack");
  });

  it("should handle multiline messages", () => {
    // Arrange
    const info: winston.Logform.TransformableInfo = {
      timestamp: "2023-01-01 12:00:00",
      level: "info",
      message: "Line 1\nLine 2\nLine 3",
    };

    // Act
    const result = logMessageFormat(info);

    // Assert
    expect(result).toBe("2023-01-01 12:00:00 info: Line 1\nLine 2\nLine 3");
  });

  it("should handle objects in message", () => {
    // Arrange
    const obj = { key: "value" };
    const info: winston.Logform.TransformableInfo = {
      timestamp: "2023-01-01 12:00:00",
      level: "info",
      message: JSON.stringify(obj),
    };

    // Act
    const result = logMessageFormat(info);

    // Assert
    expect(result).toBe('2023-01-01 12:00:00 info: {"key":"value"}');
  });

  it("should handle missing timestamp", () => {
    // Arrange
    const info: winston.Logform.TransformableInfo = {
      level: "info",
      message: "No timestamp",
    };

    // Act
    const result = logMessageFormat(info);

    // Assert
    expect(result).toBe("undefined info: No timestamp");
  });
});
