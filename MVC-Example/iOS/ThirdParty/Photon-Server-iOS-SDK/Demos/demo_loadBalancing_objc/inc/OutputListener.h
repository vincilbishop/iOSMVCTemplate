@protocol OutputListener

- (void) write:(NSString*)str, ...;
- (void) write:(NSString*)str :(va_list)args;
- (void) writeLine:(NSString*)str, ...;
- (void) writeLine:(NSString*)str :(va_list)args;

@end