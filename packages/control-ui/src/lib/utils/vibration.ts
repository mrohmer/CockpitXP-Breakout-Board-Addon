const hasVibration = () => 'vibrate' in navigator;

const vibrate = () => navigator.vibrate(500);

export const vibration = {
    hasVibration,
    vibrate,
}