/*
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */
/**
 * A Live object is a single APL Object that changes over time.  LiveMaps are
 * created and modified by ViewHosts.
 *
 * Inside of the APL document the LiveMap may be used normally in data-binding
 * contexts.  For example:
 *
 *     {
 *       "type": "Text",
 *       "text": "The live object is currently '${MyLiveMap}'"
 *     }
 *
 *  The same LiveMap may be used by multiple Context objects. This allows
 *  an application to create a single LiveMap to track a source of changing
 *  data and display it in multiple view hosts simultaneously.
 *
 *  Changing the key-value pairs in a live map will update all data-bound
 *  values currently in the component hierarchy that depend on those values.
 */
export declare class LiveMap {
    /**
     * Create a LiveMap with an initial Object
     * @param map The initial Map.
     * @return The LiveMap
     */
    static create(map?: any): LiveMap;
    liveMap: APL.LiveMap;
    private constructor();
    /**
     * Check to see if there are no elements in the map
     * @return True if the map is empty
     */
    empty(): boolean;
    /**
     * Clear all elements from the map
     */
    clear(): void;
    /**
     * Retrieve a value from the map.  If the key does not exist, a NULL object
     * will be returned.
     * @param key The key of the object to return.
     * @return The internal object.
     */
    get(key: string): any;
    /**
     * Check to see if a key exists in the map.
     * @param key The key to search for.
     * @return True if the key exists.
     */
    has(key: string): boolean;
    /**
     * Set a key-value pair in the map.
     * @param key The key to insert
     * @param value The value to store.
     */
    set(key: string, value: string): void;
    /**
     * Set a collection of values from a different map
     * @map The object map to copy values from.
     */
    update(map: any): void;
    /**
     * Replace the LiveMap with a new map.
     * @param map The new map to set.
     */
    replace(map: any): void;
    /**
     * Remove a key from the map
     * @param key The key to remove
     * @return True if the key was found and removed.
     */
    remove(key: string): boolean;
}
