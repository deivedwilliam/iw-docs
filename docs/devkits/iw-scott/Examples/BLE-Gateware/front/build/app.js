const form = document.getElementById("config-form");
const reloadButton = document.getElementById("reload-button");
const saveButton = document.getElementById("save-button");
const feedback = document.getElementById("feedback");

const configFields = {
    wifi_ssid: document.getElementById("wifi_ssid"),
    wifi_password: document.getElementById("wifi_password"),
    mqtt_uri: document.getElementById("mqtt_uri"),
    mqtt_username: document.getElementById("mqtt_username"),
    mqtt_password: document.getElementById("mqtt_password"),
    mqtt_client_id: document.getElementById("mqtt_client_id"),
    mqtt_topic: document.getElementById("mqtt_topic"),
};

const statusFields = {
    apSsid: document.getElementById("ap-ssid"),
    apIp: document.getElementById("ap-ip"),
    wifiState: document.getElementById("wifi-state"),
    wifiIp: document.getElementById("wifi-ip"),
    mqttState: document.getElementById("mqtt-state"),
    mqttError: document.getElementById("mqtt-error"),
};

function setFeedback(message, tone = "") {
    feedback.textContent = message;
    feedback.className = "feedback";
    if (tone) {
        feedback.classList.add(tone);
    }
}

function fillConfigForm(config) {
    Object.entries(configFields).forEach(([key, field]) => {
        field.value = config[key] ?? "";
    });
}

function collectConfigForm() {
    return Object.fromEntries(
        Object.entries(configFields).map(([key, field]) => [key, field.value])
    );
}

function renderStatus(status) {
    if (status.ap_enabled) {
        statusFields.apSsid.textContent = status.ap_ssid || "IW-Gateway";
        statusFields.apIp.textContent = status.ap_ip || "192.168.4.1";
    } else {
        statusFields.apSsid.textContent = "AP desligado";
        statusFields.apIp.textContent = status.sta_ip ? `Use http://${status.sta_ip}` : "AP em espera";
    }

    if (status.sta_connected) {
        statusFields.wifiState.textContent = "Conectado";
        statusFields.wifiIp.textContent = status.sta_ip || "-";
    } else if (status.sta_connecting) {
        statusFields.wifiState.textContent = "Conectando";
        statusFields.wifiIp.textContent = "Aguardando IP";
    } else {
        statusFields.wifiState.textContent = "Sem credenciais";
        statusFields.wifiIp.textContent = "-";
    }

    statusFields.mqttState.textContent = status.mqtt_connected ? "Conectado" : "Desconectado";
    statusFields.mqttError.textContent = status.mqtt_error || "Sem erro ativo";
}

async function requestJson(url, options = {}) {
    const response = await fetch(url, {
        headers: {
            "Content-Type": "application/json",
            ...(options.headers || {}),
        },
        ...options,
    });

    if (!response.ok) {
        const errorText = await response.text();
        throw new Error(errorText || `Falha em ${url}`);
    }

    return response.json();
}

async function loadConfig() {
    const config = await requestJson("/api/config");
    fillConfigForm(config);
    setFeedback("Configuracao carregada do gateway.");
}

async function loadStatus() {
    const status = await requestJson("/api/status");
    renderStatus(status);
}

async function saveConfig(event) {
    event.preventDefault();
    saveButton.disabled = true;
    setFeedback("Salvando configuracao e reaplicando Wi-Fi/MQTT...", "warning");

    try {
        const payload = collectConfigForm();
        await requestJson("/api/config", {
            method: "POST",
            body: JSON.stringify(payload),
        });
        setFeedback("Configuracao salva. O AP desliga quando o gateway receber IP no Wi-Fi.", "success");
        await loadStatus();
    } catch (error) {
        setFeedback(`Falha ao salvar: ${error.message}`, "warning");
    } finally {
        saveButton.disabled = false;
    }
}

async function reloadAll() {
    try {
        setFeedback("Recarregando informacoes do gateway...");
        await Promise.all([loadConfig(), loadStatus()]);
    } catch (error) {
        setFeedback(`Falha ao carregar: ${error.message}`, "warning");
    }
}

form.addEventListener("submit", saveConfig);
reloadButton.addEventListener("click", reloadAll);

reloadAll();
setInterval(() => {
    loadStatus().catch((error) => {
        setFeedback(`Status indisponivel: ${error.message}`, "warning");
    });
}, 5000);
