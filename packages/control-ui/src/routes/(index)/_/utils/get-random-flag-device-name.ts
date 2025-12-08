import { uniqueNamesGenerator, NumberDictionary } from 'unique-names-generator';

const realCurveNames = [
    "Eau Rouge",
    "Raidillon",
    "La Source",
    "Pouhon",
    "Blanchimont",
    "Maggots",
    "Becketts",
    "Chapel",
    "Stowe",
    "Curva Parabolica (Curva Alboreto)",
    "Variante Ascari",
    "Curva Grande",
    "Tarzanbocht",
    "Hugenholtzbocht",
    "Scheivlak",
    "130R",
    "Degner",
    "Spoon Curve",
    "The Esses",
    "Turn",
    "Sachs-Kurve",
    "Südkurve",
    "Castrol Edge",
    "Remus"
];

const genericCurveNames = [
    "Falcon Bend",
    "Apex Ridge",
    "Thunder Chicane",
    "Crimson Hairpin",
    "Horizon Curve",
    "Velocity Sweep",
    "Titan Turn",
    "Phoenix Corner",
    "Delta Chicane",
    "Ironcrest Bend",
    "Velocity Hairpin",
    "Shadow Apex",
    "Horizon Esses",
    "Razorback Curve",
    "Rivermouth Bend",
    "Solaris Corner",
    "Vortex Chicane",
    "Tempest Hairpin",
    "Thunder Apex",
    "Crestfall Bend",
    "Titan Loop",
    "Dragon’s Eye Curve",
    "Summit Hairpin",
    "Starfall Esses",
    "Panorama Bend"
];

const realTrackSectionNames = [
    "Kemmel Straight",
    "Hangar Straight",
    "Wellington Straight",
    "Mistral Straight",
    "Reta Oposta",
    "Rettifilo Tribune",
    "Casino Square",
    "Tunnel",
    "Beau Rivage",
    "Mirabeau",
    "Portier",
    "Tabac",
    "Swimming Pool Section",
    "La Rascasse",
    "Senna S",
    "Curva do Sol",
    "Laranjinha",
    "Junção",
    "Back Straight",
    "Main Straight",
    "Start-Ziel-Gerade",
    "Arena Section",
    "Stadium Section",
    "Farrington Straight",
    "Turn 14/15 Chicane"
];

const genericTrackSectionNames = [
    "Thunder Straight",
    "Apex Valley",
    "Dragon Chicane",
    "Eagle Sector",
    "Phoenix Straight",
    "Horizon Loop",
    "Crimson Sector",
    "Nebula Straight",
    "Vortex Complex",
    "Shadow Loop",
    "Iron Crest Section",
    "Titan Pass",
    "Silver Arrow Straight",
    "Delta Sector",
    "Summit Complex",
    "Starfall Straight",
    "Ridge Loop",
    "Tempest Sector",
    "Falcon Straight",
    "Solar Curve Complex",
    "Blaze Chicane",
    "Storm Ridge",
    "Aurora Straight",
    "Viper Complex",
    "Hyperion Sector"
];

const trackSectionNames = [
    ...realCurveNames,
    ...realTrackSectionNames,
    ...genericCurveNames,
    ...genericTrackSectionNames,
];

const numberDictionary = NumberDictionary.generate({ min: 1, max: 5 });
const positionSpecifiers = [
    'Start',
    'Ende',
    'Mitte',
    'Übergang',
    ...numberDictionary,
]
export const getRandomFlagDeviceName = () => uniqueNamesGenerator({
    dictionaries: [['Flagge'], trackSectionNames, positionSpecifiers],
    length: 3,
    separator: ' '
});