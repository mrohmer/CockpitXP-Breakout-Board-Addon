import type {RequestHandler} from "./$types";
import {json} from "@sveltejs/kit";
import type {ApiFlag} from "$lib/models/api-flag";
import {db} from "$lib/server/db";
import {flags} from "$lib/server/db/schema";
import {eq} from "drizzle-orm";

export const POST: RequestHandler = async ({params, request}) => {
    const {mac} = params;

    console.log(mac);

    if (!/^[0-9A-F]{2}(\:[0-9A-F]{2}){5}$/i.test(mac)) {
        return json(
            {reason: `Invalid mac address`},
            {status: 400}
        )
    }

    const macAddress = mac.toUpperCase() as string;

    const {name} = await request.json();

    if (name?.length < 5) {
        return json(
            {reason: `Invalid name`},
            {status: 400}
        )
    }

    const dbFlags = await db.select().from(flags).where(eq(flags.mac, macAddress)).limit(1);

    if (dbFlags?.length) {
        return json(
            {
                id: dbFlags[0].mac,
                name: dbFlags[0].name,
            },
            {status: 409},
        );
    }

    await db.insert(flags).values({
        name,
        mac: macAddress,
    })

    return json({id: macAddress, name} satisfies ApiFlag);
}