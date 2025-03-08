import request from "supertest";

import app from "../../src/app";

describe("Test Server Health", () => {
  it('should return 200 and "Server is healthy." on GET /', async () => {
    const response = await request(app).get("/");
    expect(response.status).toBe(200);
    expect(response.text).toBe("Server is healthy.");
  });
});
