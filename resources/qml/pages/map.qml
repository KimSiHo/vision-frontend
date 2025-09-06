import QtQuick 2.15
import QtQuick.Controls 2.15
import QtLocation 5.15
import QtPositioning 5.15

Page {
    id: mapPage

    Component.onCompleted: {
        console.info("MapPage loaded")
    }

    Component.onDestruction: {
        console.info("MapPage destroyed")
    }

    Plugin {
        id: herePlugin
        name: "here"

        PluginParameter { name: "here.app_id"; value: "<APP_ID>" }
        PluginParameter { name: "here.token"; value: "<TOKEN>" }
    }

    Plugin {
        id: osmPlugin
        name: "osm"

        PluginParameter { name: "osm.mapping.providersrepository.disabled"; value: true }
        PluginParameter { name: "osm.useragent"; value: "MyApp/1.0 (contact@example.com)" }
        PluginParameter { name: "osm.mapping.custom.host"; value: "https://a.tile.openstreetmap.org/" }
        PluginParameter { name: "osm.mapping.custom.host"; value: "https://b.tile.openstreetmap.org/" }
        PluginParameter { name: "osm.mapping.custom.host"; value: "https://c.tile.openstreetmap.org/" }
        PluginParameter { name: "osm.mapping.copyright"; value: "© OpenStreetMap contributors" }

        PluginParameter { name: "osm.places.debug_query"; value: true }
        PluginParameter { name: "osm.geocoding.debug_query"; value: true }

        PluginParameter {name: "osm.mapping.cache.directory"; value: "/tmp/mapcache"}
        PluginParameter { name: "osm.mapping.cache.disk.size"; value: 100000000 }
        PluginParameter { name: "osm.mapping.cache.memory.size"; value: 20000000 }
    }

    Map {
        anchors.fill: parent
        plugin: osmPlugin
        center: QtPositioning.coordinate(37.5665, 126.9780)
        zoomLevel: 12
        activeMapType: supportedMapTypes[supportedMapTypes.length - 1]

        gesture.enabled: true
    }
}
