export interface Notification {
    id: string;
    type: 'success' | 'progress' | 'error';
    progress?: number;
    text: string;
    dismissAfter?: Date;
    action?: {
        text: string;
        action: () => void;
    }
}

export const createSuccess = (id: string, text: string): Notification => ({
    id,
    type: 'success',
    text,
});
export const createError = (id: string, text: string): Notification => ({
    id,
    type: 'error',
    text,
});
export const createProgress = (id: string, text: string): Notification => ({
    id,
    type: 'progress',
    text,
});
export const dismissAfter = (notification: Notification, ms: number): Notification => ({
    ...notification,
    dismissAfter: new Date(+new Date() + ms),
})