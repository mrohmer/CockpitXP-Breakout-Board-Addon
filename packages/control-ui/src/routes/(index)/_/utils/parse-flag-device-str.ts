import type {FlagDevice} from "../models/flag-device";

const idToMac = (id: string): string => [
    [id[0], id[1]],
    [id[2], id[3]],
    [id[4], id[5]],
    [id[6], id[7]],
    [id[8], id[9]],
    [id[10], id[11]],
].map((d) => d.join('')).join(':');
const parseFlagDeviceSubStr = (str: string): FlagDevice|undefined => {
    const result = /^(?<id>[A-Z0-9]{12})(?<online>[0-2])(?<battery>[1-9]\d?|100)$/.exec(str);

    if (!result?.groups) {
        return undefined;
    }

    const id = result.groups.id;

    return {
        id,
        mac: idToMac(id),
        battery: +result.groups.battery,
        state: +result.groups.online,
    }
}
export const parseFlagDeviceStr = (str: string): FlagDevice[] => {
    if (!str?.trim?.()) return [];

    return str.split(";").map((i) => parseFlagDeviceSubStr(i.trim())).filter(Boolean) as FlagDevice[];
}