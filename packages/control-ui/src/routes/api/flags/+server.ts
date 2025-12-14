import type {RequestHandler} from "./$types";
import {json} from "@sveltejs/kit";
import {flags} from "$lib/server/db/schema";
import {db} from "$lib/server/db";
import type {ApiFlag} from "$lib/models/api-flag";

export const GET: RequestHandler = async () => {
    const dbResult = await db.select().from(flags);

    const response = dbResult.map(flag => ({
        id: flag.mac,
        name: flag.name,
    } satisfies ApiFlag));

    return json({
        flags: response,
    });
}