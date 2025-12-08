export enum OnlineState {
    ONLINE = 2,
    HEARTBEAT_MISSED = 1,
    OFFLINE = 0,
}

export interface FlagDevice {
    id: string;
    mac: string;
    battery: number;
    state: OnlineState;
}