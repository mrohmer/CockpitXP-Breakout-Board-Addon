export const truncate = (str: string, maxLength: number) => {
    str = str?.trim?.();

    if (!str || maxLength <= 3 || str.length <= maxLength) {
        return str;
    }

    return `${str.substring(0, maxLength - 3)}...`;
}