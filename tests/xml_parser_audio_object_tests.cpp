#define CATCH_CONFIG_ENABLE_CHRONO_STRINGMAKER
#include <catch2/catch.hpp>
#include <sstream>
#include "adm/document.hpp"
#include "adm/elements/audio_object.hpp"
#include "adm/elements/position_offset.hpp"
#include "adm/parse.hpp"
#include "adm/errors.hpp"

TEST_CASE("xml_parser/audio_object") {
  using namespace adm;
  auto document = parseXml("xml_parser/audio_object.xml");
  auto audioObject = document->lookup(parseAudioObjectId("AO_1001"));

  REQUIRE(audioObject->has<AudioObjectName>() == true);
  REQUIRE(audioObject->has<AudioObjectId>() == true);
  REQUIRE(audioObject->has<Start>() == true);
  REQUIRE(audioObject->has<Duration>() == true);
  REQUIRE(audioObject->has<DialogueId>() == true);
  REQUIRE(audioObject->has<Importance>() == true);
  REQUIRE(audioObject->has<Interact>() == true);
  REQUIRE(audioObject->has<DisableDucking>() == true);

  REQUIRE(audioObject->get<AudioObjectName>() == "MyObject");
  REQUIRE(audioObject->get<AudioObjectId>().get<AudioObjectIdValue>() ==
          0x1001u);
  REQUIRE(audioObject->get<Start>().get() == std::chrono::seconds(0));
  REQUIRE(audioObject->get<Duration>().get() == std::chrono::seconds(10));
  REQUIRE(audioObject->get<DialogueId>() == Dialogue::NON_DIALOGUE);
  REQUIRE(audioObject->get<Importance>() == 10);
  REQUIRE(audioObject->get<Interact>() == false);
  REQUIRE(audioObject->get<DisableDucking>() == true);
  REQUIRE(audioObject->get<HeadLocked>() == true);

  auto labels = audioObject->get<Labels>();
  REQUIRE(labels.size() == 2);
  REQUIRE(labels[0].get<LabelLanguage>() == "en");
  REQUIRE(labels[0].get<LabelValue>() == "My Object");
  REQUIRE(labels[1].get<LabelLanguage>() == "deu");
  REQUIRE(labels[1].get<LabelValue>() == "Mein Objekt");

  REQUIRE(audioObject->get<Gain>().asLinear() == 0.5);

  REQUIRE(audioObject->has<PositionOffset>() == false);
  REQUIRE(audioObject->has<SphericalPositionOffset>() == false);
  REQUIRE(audioObject->has<CartesianPositionOffset>() == false);

  REQUIRE(audioObject->has<Mute>() == true);
  REQUIRE(audioObject->get<Mute>() == true);
}

TEST_CASE("xml_parser/audio_object_duplicate_id") {
  REQUIRE_THROWS_AS(adm::parseXml("xml_parser/audio_object_duplicate_id.xml"),
                    adm::error::XmlParsingDuplicateId);
}

TEST_CASE("xml_parser/audio_object_interaction") {
  using namespace adm;
  auto document = adm::parseXml("xml_parser/audio_object_interaction.xml");
  auto audioObjects = document->getElements<AudioObject>();
  REQUIRE(audioObjects.size() == 2);

  auto audioObject_0 = document->lookup(parseAudioObjectId("AO_1001"));
  auto audioObject_1 = document->lookup(parseAudioObjectId("AO_1002"));

  REQUIRE(audioObject_0->has<AudioObjectInteraction>() == true);
  auto interaction_0 = audioObjects[0]->get<AudioObjectInteraction>();
  REQUIRE(interaction_0.get<OnOffInteract>() == true);
  REQUIRE(interaction_0.has<GainInteract>() == true);
  REQUIRE(interaction_0.get<GainInteract>() == true);
  REQUIRE(interaction_0.has<PositionInteract>() == true);
  REQUIRE(interaction_0.get<PositionInteract>() == true);
  REQUIRE(interaction_0.has<GainInteractionRange>() == true);
  auto gain_interaction_0 = interaction_0.get<GainInteractionRange>();
  REQUIRE(gain_interaction_0.has<GainInteractionMin>() == true);
  REQUIRE(gain_interaction_0.has<GainInteractionMax>() == true);
  REQUIRE(gain_interaction_0.get<GainInteractionMin>().get().asLinear() ==
          Approx(0.5f));
  REQUIRE(gain_interaction_0.get<GainInteractionMax>().get().asLinear() ==
          Approx(1.5f));
  REQUIRE(interaction_0.has<PositionInteractionRange>() == true);
  auto position_interaction_0 = interaction_0.get<PositionInteractionRange>();
  REQUIRE(position_interaction_0.has<AzimuthInteractionMin>() == true);
  REQUIRE(position_interaction_0.has<AzimuthInteractionMax>() == true);
  REQUIRE(position_interaction_0.get<AzimuthInteractionMin>() == Approx(-30.f));
  REQUIRE(position_interaction_0.get<AzimuthInteractionMax>() == Approx(30.f));
  REQUIRE(position_interaction_0.has<ElevationInteractionMin>() == true);
  REQUIRE(position_interaction_0.has<ElevationInteractionMax>() == true);
  REQUIRE(position_interaction_0.get<ElevationInteractionMin>() ==
          Approx(-45.f));
  REQUIRE(position_interaction_0.get<ElevationInteractionMax>() ==
          Approx(45.f));
  REQUIRE(position_interaction_0.has<DistanceInteractionMin>() == true);
  REQUIRE(position_interaction_0.has<DistanceInteractionMax>() == true);
  REQUIRE(position_interaction_0.get<DistanceInteractionMin>() == Approx(0.5f));
  REQUIRE(position_interaction_0.get<DistanceInteractionMax>() == Approx(1.5f));

  REQUIRE(audioObject_1->has<AudioObjectInteraction>() == true);
  auto interaction_1 = audioObjects[1]->get<AudioObjectInteraction>();
  REQUIRE(interaction_1.get<OnOffInteract>() == true);
  REQUIRE(interaction_1.has<GainInteract>() == true);
  REQUIRE(interaction_1.get<GainInteract>() == true);
  REQUIRE(interaction_1.has<PositionInteract>() == true);
  REQUIRE(interaction_1.get<PositionInteract>() == true);
  REQUIRE(interaction_1.has<GainInteractionRange>() == true);
  auto gain_interaction_1 = interaction_1.get<GainInteractionRange>();
  REQUIRE(gain_interaction_1.has<GainInteractionMin>() == true);
  REQUIRE(gain_interaction_1.has<GainInteractionMax>() == true);
  REQUIRE(gain_interaction_1.get<GainInteractionMin>().get().asLinear() ==
          Approx(0.5f));
  REQUIRE(gain_interaction_1.get<GainInteractionMax>().get().asLinear() ==
          Approx(1.5f));
  REQUIRE(interaction_1.has<PositionInteractionRange>() == true);
  auto position_interaction_1 = interaction_1.get<PositionInteractionRange>();
  REQUIRE(position_interaction_1.has<XInteractionMin>() == true);
  REQUIRE(position_interaction_1.has<XInteractionMax>() == true);
  REQUIRE(position_interaction_1.get<XInteractionMin>() == Approx(-1.f));
  REQUIRE(position_interaction_1.get<XInteractionMax>() == Approx(1.f));
  REQUIRE(position_interaction_1.has<YInteractionMin>() == true);
  REQUIRE(position_interaction_1.has<YInteractionMax>() == true);
  REQUIRE(position_interaction_1.get<YInteractionMin>() == Approx(-1.f));
  REQUIRE(position_interaction_1.get<YInteractionMax>() == Approx(1.f));
  REQUIRE(position_interaction_1.has<ZInteractionMin>() == true);
  REQUIRE(position_interaction_1.has<ZInteractionMax>() == true);
  REQUIRE(position_interaction_1.get<ZInteractionMin>() == Approx(-1.f));
  REQUIRE(position_interaction_1.get<ZInteractionMax>() == Approx(1.f));
}

TEST_CASE("xml_parser/audio_object_position_offset") {
  using namespace adm;
  auto document = parseXml("xml_parser/audio_object_position_offset.xml");

  {
    // Spherical position offset
    auto audioObject = document->lookup(parseAudioObjectId("AO_1001"));
    REQUIRE(audioObject->has<PositionOffset>() == true);
    REQUIRE(audioObject->has<SphericalPositionOffset>() == true);
    REQUIRE(audioObject->has<CartesianPositionOffset>() == false);

    auto positionOffset = audioObject->get<SphericalPositionOffset>();
    REQUIRE(positionOffset.get<AzimuthOffset>() == Approx(30.0f));
    REQUIRE(positionOffset.get<ElevationOffset>() == Approx(15.0f));
    REQUIRE(positionOffset.get<DistanceOffset>() == Approx(0.9f));
  }

  {
    // Cartesian position offset
    auto audioObject = document->lookup(parseAudioObjectId("AO_1002"));
    REQUIRE(audioObject->has<PositionOffset>() == true);
    REQUIRE(audioObject->has<SphericalPositionOffset>() == false);
    REQUIRE(audioObject->has<CartesianPositionOffset>() == true);

    auto positionOffset = audioObject->get<CartesianPositionOffset>();
    REQUIRE(positionOffset.get<XOffset>() == Approx(-0.2f));
    REQUIRE(positionOffset.get<YOffset>() == Approx(0.1f));
    REQUIRE(positionOffset.get<ZOffset>() == Approx(-0.5f));
  }

  {
    // empty spherical position offset
    auto audioObject = document->lookup(parseAudioObjectId("AO_1003"));
    REQUIRE(audioObject->has<PositionOffset>() == true);
    REQUIRE(audioObject->has<SphericalPositionOffset>() == true);
    REQUIRE(audioObject->has<CartesianPositionOffset>() == false);

    auto positionOffset = audioObject->get<SphericalPositionOffset>();
    REQUIRE(positionOffset.has<AzimuthOffset>());
    REQUIRE(positionOffset.get<AzimuthOffset>() == Approx(30.0f));
    REQUIRE(!positionOffset.has<ElevationOffset>());
    REQUIRE(!positionOffset.has<DistanceOffset>());
  }

  {
    // empty Cartesian position offset
    auto audioObject = document->lookup(parseAudioObjectId("AO_1004"));
    REQUIRE(audioObject->has<PositionOffset>() == true);
    REQUIRE(audioObject->has<SphericalPositionOffset>() == false);
    REQUIRE(audioObject->has<CartesianPositionOffset>() == true);

    auto positionOffset = audioObject->get<CartesianPositionOffset>();
    REQUIRE(positionOffset.has<XOffset>());
    REQUIRE(positionOffset.get<XOffset>() == Approx(-0.2f));
    REQUIRE(!positionOffset.has<YOffset>());
    REQUIRE(!positionOffset.has<ZOffset>());
  }

  {
    // empty object
    auto audioObject = document->lookup(parseAudioObjectId("AO_1005"));
    REQUIRE(audioObject->has<PositionOffset>() == false);
  }
}
