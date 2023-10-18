#include <mosquitto.h>
#include <stdio.h>

#define HOST ""
#define PORT 8883
#define USER ""
#define PASS ""

#define SSL_VERIFY_PEER 1

int hello_world = 6999;

typedef enum {
    HELLO_WORLD = 69000
} topics_t;


void on_message(struct mosquitto *mosq, void *obj, const struct mosquitto_message *message, const mosquitto_property* prop) {
    // printf("Message received\n");
    printf("Message received on topic %s: %s\n", message->topic, message->payload);
}

void on_connect(struct mosquitto *mosq, void *obj, int rc, int flags, const mosquitto_property *props) {
    if (rc == 0) {
        printf("Connected to broker\n");
        if (mosquitto_subscribe_v5(mosq, NULL, "test", 0, 0, NULL) != MOSQ_ERR_SUCCESS) {
            printf("Error subscribing.\n");
        }
    } else {
        printf("Connection failed\n");
    }
}

int main() {
    int err;
    struct mosquitto *mosq = NULL;
    mosquitto_lib_init();

    mosq = mosquitto_new("test_client", true, NULL);
    if (!mosq) {
        fprintf(stderr, "Error: Out of memory.\n");
        return 1;
    }

    if (err = mosquitto_int_option(mosq, MOSQ_OPT_PROTOCOL_VERSION, MQTT_PROTOCOL_V5)){
        printf("Error setting mqtt opts: %s\n", mosquitto_strerror(err));
        return 1;
    }

    if (err = mosquitto_username_pw_set(mosq, USER, PASS)) {
        printf("Error setting username/pass: %s\n", mosquitto_strerror(err));
        return 1;
    }
    
    // tls
    if (err = mosquitto_tls_opts_set(mosq, SSL_VERIFY_PEER, "tlsv1.2", NULL)){
        printf("Error setting tls opts: %s\n", mosquitto_strerror(err));
        return 1;
    }

    if (err = mosquitto_tls_set(mosq, "ca.crt", NULL, NULL, NULL, NULL)) {
        printf("Error setting CA: %s\n", mosquitto_strerror(err));
        return 1;
    }


    mosquitto_connect_v5_callback_set(mosq, on_connect);
    mosquitto_message_v5_callback_set(mosq, on_message);
    
    mosquitto_connect(mosq, HOST, PORT, 10);

    mosquitto_loop_forever(mosq, -1, 1);

    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();
    
    return 0;
}