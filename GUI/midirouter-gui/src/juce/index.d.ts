declare module "juce" {
    /**
     * Obtiene una función nativa desde el backend.
     * @param name - El nombre de la función nativa.
     * @returns Una función que devuelve una promesa con el resultado.
     */
    export function getNativeFunction(name: string): (...args: any[]) => Promise<any>;

    /**
     * Obtiene el estado actual de un slider.
     * @param name - El identificador único del slider.
     * @returns Un objeto SliderState que representa el estado del slider.
     */
    export function getSliderState(name: string): SliderState;

    /**
     * Obtiene el estado actual de un toggle (interruptor).
     * @param name - El identificador único del toggle.
     * @returns Un objeto ToggleState que representa el estado del toggle.
     */
    export function getToggleState(name: string): ToggleState;

    /**
     * Obtiene el estado actual de un ComboBox.
     * @param name - El identificador único del ComboBox.
     * @returns Un objeto ComboBoxState que representa el estado del ComboBox.
     */
    export function getComboBoxState(name: string): ComboBoxState;

    /**
     * Obtiene la dirección de un recurso en el backend.
     * @param path - La ruta del recurso.
     * @returns La dirección del recurso (string).
     */
    export function getBackendResourceAddress(path: string): string;

    /**
     * Clase para actualizar el índice de un parámetro de control.
     * @param controlParameterIndexAnnotation - El atributo que identifica el índice del parámetro de control.
     */
    export class ControlParameterIndexUpdater {
        constructor(controlParameterIndexAnnotation: string);
        handleMouseMove(event: MouseEvent): void;
    }
}

// Definiciones adicionales para SliderState, ToggleState y ComboBoxState

interface SliderState {
    name: string;
    identifier: string;
    scaledValue: number;
    properties: {
        start: number;
        end: number;
        skew: number;
        name: string;
        label: string;
        numSteps: number;
        interval: number;
        parameterIndex: number;
    };
    valueChangedEvent: ListenerList;
    propertiesChangedEvent: ListenerList;

    setNormalisedValue(newValue: number): void;
    sliderDragStarted(): void;
    sliderDragEnded(): void;
    getScaledValue(): number;
    getNormalisedValue(): number;
}

interface ToggleState {
    name: string;
    identifier: string;
    value: boolean;
    properties: {
        name: string;
        parameterIndex: number;
    };
    valueChangedEvent: ListenerList;
    propertiesChangedEvent: ListenerList;

    getValue(): boolean;
    setValue(newValue: boolean): void;
}

interface ComboBoxState {
    name: string;
    identifier: string;
    value: number;
    properties: {
        name: string;
        parameterIndex: number;
        choices: string[];
    };
    valueChangedEvent: ListenerList;
    propertiesChangedEvent: ListenerList;

    getChoiceIndex(): number;
    setChoiceIndex(index: number): void;
}

declare class ListenerList {
    private listeners: Map<number, (payload: any) => void>;
    private listenerId: number;

    constructor();
    addListener(fn: (payload: any) => void): number;
    removeListener(id: number): void;
    callListeners(payload: any): void;
}