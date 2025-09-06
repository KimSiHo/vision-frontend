import QtQuick 2.15
import QtQuick.Controls 2.15
import QtLocation 5.15
import QtPositioning 5.15

Page {
    id: mapPage

    Plugin {
        id: mapPlugin
        name: "mapboxgl"

        // 🔑 반드시 넣어야 함 (없으면 "API key required" 에러)
        PluginParameter {
            name: "mapboxgl.access_token"
            value: "YOUR_MAPBOX_ACCESS_TOKEN"
        }

        // 🔹 스타일 (streets, satellite, outdoors, light, dark)
        PluginParameter {
            name: "mapboxgl.mapbox.map_id"
            value: "mapbox/streets-v11"
        }

        // 캐시 경로 (sqlite 필요)
        PluginParameter {
            name: "mapboxgl.cache.dbname"
            value: "/tmp/mapbox-cache.sqlite"
        }
    }

    Map {
        id: mapView
        anchors.fill: parent
        plugin: mapPlugin
        zoomLevel: 13
        center: QtPositioning.coordinate(37.5665, 126.9780) // 서울 시청
    }
}
