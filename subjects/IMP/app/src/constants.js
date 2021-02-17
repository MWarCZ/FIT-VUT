
const ESP_BASE_URL = 'http://192.168.4.1'

const axiosConfig = {
  // withCredentials: true,
  headers: {
    // 'Content-Type': 'application/x-www-form-urlencoded'
    // 'Content-Type': 'text/plain',
    'Access-Control-Allow-Origin': '*',
  }
}

export {
  ESP_BASE_URL,
  axiosConfig
}
