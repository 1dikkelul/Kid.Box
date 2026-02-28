#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

// Simple status struct so main app can check WiFi state
typedef struct {
    bool started;
    bool connected;
    bool got_ip;
    uint32_t ip; // Optional: store IP as uint32_t for easy printing (use esp_ip4addr_ntoa to convert to string)
} wifi_manager_status_t;

// Start WiFi in station mode (non-blocking after init)
void wifi_manager_start_sta(void);

// Get current status anytime
wifi_manager_status_t wifi_manager_get_status(void);

// stuff for the IP Address
uint32_t wifi_manager_get_ip(void);
void wifi_manager_get_ip_str(char *buf, size_t buflen);
bool wifi_manager_is_ready(void);

#ifdef __cplusplus
}
#endif