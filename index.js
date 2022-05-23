console.log("API para ESP32")
//const { response } = require("express");
const express = require("express");
//const req = require("express/lib/request");
const app  = express();
const port = 3456;

app.use(express.json());

app.post('/esp32', (req, res) => {
  console.log(req.body);
  res.send('Hola mi server en express');
});

//routerAPI(app);

app.listen(port, () => {
  console.log("Escuchando en: localhost:3001")
});


// connection.query(`INSERT INTO contador_reg_bloqueos (clase, id_dispositivo, status, tipo,elemento_bloqueado, evento, fecha_hora,  ruta, sensor1_est_bloq, sensor2_est_bloq, sensor3_est_bloq, sensor4_est_bloq, sensor5_est_bloq, sensor6_est_bloq, lin1_est_bloq, lin2_est_bloq, lin3_est_bloq, bloqueo_total, ramal) VALUES(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)`,
//   [body.clase, body.id_dispositivo, body.status,body.Tipo,, body.Elemento_Bloqueado, body.Evento, body.Fecha_hora, body.sensor_1_EstadoBloqueo, body.sensor_2_EstadoBloqueo, body.sensor_3_EstadoBloqueo, body.sensor_4_EstadoBloqueo, body.sensor_5_EstadoBloqueo, body.sensor_6_EstadoBloqueo, body.Linea_1_EstadoBloqueo, body.Linea_2_EstadoBloqueo, body.Linea_3_EstadoBloqueo, body.Bloqueo_Total, body.ruta,body.unidad,body.ramal],(error,
//   results) => {
//     if (error) return res.json({ error: error });
//     });
