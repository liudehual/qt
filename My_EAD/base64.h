#ifndef BASE64_H
#define BASE64_H


class base64
{
public:
  base64();
  int Base64encode_len(int len);
  int Base64encode(char * coded_dst, const char *plain_src,int len_plain_src);

  int Base64decode_len(const char * coded_src);
  int Base64decode(char * plain_dst, const char *coded_src);
};

#endif // BASE64_H
