import { MigrationInterface, QueryRunner } from "typeorm";

export class CreateUserTable1741890802812 implements MigrationInterface {
    name = 'CreateUserTable1741890802812'

    public async up(queryRunner: QueryRunner): Promise<void> {
        await queryRunner.query(`CREATE TYPE "public"."users_role_enum" AS ENUM('admin', 'user')`);
        await queryRunner.query(`CREATE TABLE "users" ("id" uuid NOT NULL DEFAULT uuid_generate_v4(), "username" character varying NOT NULL, "password" character varying NOT NULL, "name" character varying NOT NULL, "ip" character varying NOT NULL, "limit" bigint, "role" "public"."users_role_enum" NOT NULL DEFAULT 'user', "createdAt" TIMESTAMP WITH TIME ZONE NOT NULL DEFAULT now(), "updatedAt" TIMESTAMP WITH TIME ZONE NOT NULL DEFAULT now(), CONSTRAINT "UQ_fe0bb3f6520ee0469504521e710" UNIQUE ("username"), CONSTRAINT "UQ_51b8b26ac168fbe7d6f5653e6cf" UNIQUE ("name"), CONSTRAINT "UQ_da975f2f2d61d876ff98acff765" UNIQUE ("ip"), CONSTRAINT "PK_a3ffb1c0c8416b9fc6f907b7433" PRIMARY KEY ("id"))`);
    }

    public async down(queryRunner: QueryRunner): Promise<void> {
        await queryRunner.query(`DROP TABLE "users"`);
        await queryRunner.query(`DROP TYPE "public"."users_role_enum"`);
    }

}
