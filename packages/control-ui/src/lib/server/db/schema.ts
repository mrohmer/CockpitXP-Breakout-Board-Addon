import {sqliteTable, text} from 'drizzle-orm/sqlite-core';
import {sql} from "drizzle-orm";

export const flags = sqliteTable('flags', {
	id: text('id').primaryKey().$defaultFn(() => crypto.randomUUID()),
	createdAt: text('timestamp')
        .notNull()
        .default(sql`(current_timestamp)`),
    name: text('name'),
    mac: text('mac').unique(),
});
