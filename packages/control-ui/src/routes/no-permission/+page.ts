import type {PageLoad} from "./$types";
import {bluetooth} from "$lib";
import {redirect} from "@sveltejs/kit";
import {browser} from "$app/environment";

export const load: PageLoad = async () => {
    if (browser && await bluetooth.hasPermission()) {
        throw redirect(302, '/');
    }

    return {};
}