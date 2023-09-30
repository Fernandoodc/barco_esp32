let SERVERIP = "0.0.0.0"; //IP del servidor, se usa este si el usuario no proporciona uno
let PGIRO = 50; //porcentaje de giro
let SPEED = 0; //velocidad del motor
$(document).ready(function () {
    if (typeof (Storage) !== "undefined") {
        SERVERIP = localStorage.getItem("serverIP");
        $("#serverIP").val(SERVERIP);
    }
    setInterval(renovarVida, 500);
    //para que el form no haga nada
    $(".form").submit(function (e) {
        e.preventDefault();
    });
    $('#serverIP').keypress(function (e) {
        $(".okIP").show();
    });
    $('#serverIP-form').submit(function (e) {
        e.preventDefault();
        SERVERIP = $("#serverIP").val();
        if (typeof (Storage) !== "undefined") {
            localStorage.setItem("serverIP", SERVERIP);
        }
        $(".okIP").hide();
    });
    $("#adelante").click(function (e) {
        e.preventDefault();
        let data = {
            stop: false
        };
        let url = SERVERIP + "/stop"
        ajax(url, data)
        data = {
            porcen: $('#speed').val(),
            reverse: false
        };
        url = SERVERIP + "/speed"
        ajax(url, data)
    });
    $("#atras").click(function (e) {
        e.preventDefault();
        let data = {
            porcen: 0,
            reverse: true
        };
        let url = SERVERIP + "/speed"
        ajax(url, data)
        setTimeout(function(){
            let data = {
                porcen: $('#speed').val(),
                reverse: true
            };
            let url = SERVERIP + "/speed"
            ajax(url, data)
        }, 500) //espera medio segundo para girar de nuevo el motor
    });
    $("#parar").click(function (e) {
        e.preventDefault();
        let data = {
            stop: true
        };
        let url = SERVERIP + "/stop"
        ajax(url, data)
    });
    $("#izquierda").click(function (e) {
        e.preventDefault();
        let data = {
            dir: "L",
            porcen: PGIRO
        };
        let url = SERVERIP + "/direction"
        ajax(url, data)
    });
    $("#derecha").click(function (e) {
        e.preventDefault();
        let data = {
            dir: "R",
            porcen: PGIRO
        };
        let url = SERVERIP + "/direction"
        ajax(url, data)
    });
    $("#recto").click(function (e) {
        e.preventDefault();
        let data = {
            dir: "L",
            porcen: 0
        };
        let url = SERVERIP + "/direction"
        ajax(url, data)
    });
    $('#speed-form').submit(function (e) {
        e.preventDefault();
        SPEED = $('#speed').val();
        let data = {
            porcen: SPEED,
            reverse: false,
        };
        let url = SERVERIP + "/speed"
        ajax(url, data);
    });
    $('#speed').change(function (e) {
        e.preventDefault();
        console.log($(this).val())
    });
});
/**
 * realiza una petición de tipo GET usando ajax 
 * @param {string} url dirección de la petición pero sin el http:// por ejemplo 0.0.0.0/direcion
 * @param {JSON} data datos a enviar por GET
 */
function ajax(url, data) {
    url = protocoloUrl(url)
    console.log(url)
    $.ajax({
        type: "get",
        url: url,
        data: data,
        success: function (response) {
            console.log(response)
        },
        error: function (e) {
            console.log(e);
            alert("Ocurrió un error")
        }
    });
}

function renovarVida() {
    let url = protocoloUrl(SERVERIP + "/alive");
    console.log(url)
    $.ajax({
        type: "GET",
        url: url,
        success: function (response) {
        }
    });
}

function protocoloUrl(url) {
    if (url.startsWith("http://") || url.startsWith("https://")) {
        return url;
    }else {
        return "http://" + url;
    }
}

