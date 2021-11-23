//**Le fichier JS principale de la page de l'application*/
/**
 * @author Daniel Boisclair
 * @version 1.0
 * @modifications
 *              Aucune en date du 03 Septembre 2021
 */

/** Liste du bois */
var listeBoisInformations = {};
var currentBoisId = null;

var ovenIsStarted = false;
var ovenEnabled = true;
var intervalCookingTime = null;

var demarrerFourElement = getElement("#demarrerFour");

const routeAPI = "http://10.0.0.53/";

/**Fonction qui permet d'envoyer une requête par la méthode GET */
function sendGETRequest(apiRequest, params, callback) {
    let paramsString = "?";
    for (const [key, value] of Object.entries(params)) {
        paramsString += key + "=" + value + "&";
    }
    paramsString = paramsString.slice(0, -1);

    var xhr = new XMLHttpRequest();
    xhr.onreadystatechange = callback;
    xhr.open("GET", routeAPI + apiRequest + paramsString, true);
    xhr.setRequestHeader('Content-Type', 'application/json');
    xhr.setRequestHeader('Access-Control-Allow-Headers', '*');
    xhr.setRequestHeader('Access-Control-Allow-Origin', '*');
    xhr.send();
}

/**Fonction qui permet d'obtenir un élément par le query du document */
function getElement(query) {
    return document.querySelector(query);
}

/**Fonction qui affiche les détails du bois */
function afficherDetailsBois(idBois) {
    currentBoisId = idBois;

    let bois = listeBoisInformations[idBois];
    getElement("#bois").innerHTML = bois.nom;
    getElement("#type").innerHTML = bois.type;
    getElement("#origine").innerHTML = bois.origine;
    getElement("#tempsSechage").innerHTML = bois.tempsSechage;
    getElement("#tempMin").innerHTML = bois.tempMin;

    getElement("#typeBois").innerHTML = bois.nom;
    getElement("#tempMinFour").innerHTML = bois.tempMin;
    getElement("#tempsTotal").innerHTML = bois.tempsSechage;

}

function stopOven() {
    ovenIsStarted = false;
    ovenEnabled = true;

    if (intervalCookingTime) {
        clearInterval(intervalCookingTime);
        demarrerFourElement.innerHTML = "Démarrage du four";
        getElement("#tempVal").innerHTML = "-";
        getElement("#tempsActuel").innerHTML = "0";
    }
}

function setCurrentCookingTime() {
    sendGETRequest("getOvenCookingInformations", {}, function() {
        if (this.readyState != 4) return;
        if (this.status == 200) {
            let ovenCookingInf = JSON.parse(this.responseText);
            if (!ovenCookingInf.isStarted)
                return stopOven();

            getElement("#tempsActuel").innerHTML = ovenCookingInf.time;
            getElement("#tempVal").innerHTML = ovenCookingInf.temp;
        }
    });
}

/**Event du chargement de la page */
window.onload = function() {
    /**L'élément select */
    var selectionBois = document.querySelector("#selectionBois");

    /**Fonction qui permet d'obtenir tous le bois dans la BDD */
    sendGETRequest("getAllWoodOptions", {}, function() {
        if (this.readyState != 4) return;
        if (this.status == 200) {
            var listeBois = JSON.parse(this.responseText);
            if (listeBois) {
                listeBois.forEach(function(bois) {
                    selectionBois.innerHTML += ("<option value='" + bois.id + "'>" + bois.nom +"</option>");

                    listeBoisInformations[bois.id] = bois;
                });

                afficherDetailsBois(listeBois[0].id);
            } else {
                console.log("err");
            }
        }
    });

    /**Fonciton qui permet de rafraichir les données du bois sélectionné */
    selectionBois.addEventListener("change", function() {
        afficherDetailsBois(this.value);
    });
    
    demarrerFourElement.onclick = function() {
        if (!ovenEnabled) { return; }

        let bois = listeBoisInformations[currentBoisId];
        ovenEnabled = false;

        if (!ovenIsStarted) {
            sendGETRequest("startOven", {
                "cookingTime": bois.tempsSechage,
                "minTemp": bois.tempMin
            }, function() {
                if (this.readyState != 4) return;
                ovenIsStarted = this.status == 200;
                ovenEnabled = true;

                if (ovenIsStarted) {
                    intervalCookingTime = setInterval(setCurrentCookingTime, 1000);
                    demarrerFourElement.innerHTML = "Arrêt du four";
                }
            });
        } else {
            sendGETRequest("stopOven", {}, function() {
                if (this.readyState != 4) return;
                stopOven();
            });
        }
        
    };
}