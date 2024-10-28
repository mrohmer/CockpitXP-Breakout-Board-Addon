export const request = async (path: string, options: RequestInit = {}) => {
  const response = await fetch(path, options);
  if (!response.ok) {
     throw new Error(`HTTP error! status: ${response.status}`);
  }
  return response.json();
}
