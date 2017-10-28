//
//  CString.swift
//  GuomiDemo
//
//  Created by 王军建 on 2017/10/27.
//  Copyright © 2017年 狗吃草. All rights reserved.
//

import Foundation

private let stringDefaultSize = 10000

class CString {
    var strPtr: UnsafeMutablePointer<Int8>
    var strSize: Int = 0
    
    init(size: Int = stringDefaultSize) {
        strSize = size
        strPtr = UnsafeMutablePointer<Int8>.allocate(capacity: strSize)

        initStringValueByZero()
    }
    
    private func initStringValueByZero() {
        for i in 0..<strSize {
            strPtr.advanced(by: i).pointee = 0
        }
    }

    deinit {
        strPtr.deallocate(capacity: strSize)
    }
    
    public func toPtr() -> UnsafeMutablePointer<Int8> {
        return strPtr
    }
    
    public func toString() -> String {
        return String.init(cString: strPtr)
    }
}

class CUString {
    var strPtr: UnsafeMutablePointer<UInt8>
    var strSize: Int = 0
    
    init(size: Int = stringDefaultSize) {
        strSize = size
        strPtr = UnsafeMutablePointer<UInt8>.allocate(capacity: strSize)
        
        initStringValueByZero()
    }
    
    private func initStringValueByZero() {
        for i in 0..<strSize {
            strPtr.advanced(by: i).pointee = 0
        }
    }
    
    deinit {
        strPtr.deallocate(capacity: strSize)
    }
    
    public func toPtr() -> UnsafeMutablePointer<UInt8> {
        return strPtr
    }
    
    public func toString() -> String {
        return String.init(cString: strPtr)
    }
}
